#include <iostream>
#include <stack>
#include <string>
#include <cstdlib>
using namespace std;

//Calculation functions
double calcInfix();
double calcPrefix();
double calcPostfix();
double calcAnswer(string postfix);

//Conversion functions
string infixToPostfix(string infix);
string prefixToInfix(string prefix);

//Misc. functions
string getExpression();           //Gets the expression
void reverse(string &sReverse);   //Reverses a prefix expression
bool isOperator(char input);      //Checks whether input is operator
int precedence(char input);       //Returns operator precedence

//Constant characters
const char LEFT_PAREN = '(';
const char RIGHT_PAREN = ')';
const char DECIMAL = '.';
const char SPACE = ' ';

int main()
{
	//Expression types
	enum expression { none, infix, prefix, postfix };
	expression type = none;

	string input;

	//Get expression type
	cout << "Enter the expression type (INFIX, PREFIX, or POSTFIX)." << endl;
	cin >> input;
	cin.ignore(256, '\n');
	cout << endl;

	//Convert input to uppercase
	for (unsigned int i = 0; i < input.length();  i++)
		input[i] = toupper(input[i]);

	//Set type according to input
	if (input.compare("INFIX") == 0)
		type = infix;

	else if (input.compare("PREFIX") == 0)
		type = prefix;

	else if (input.compare("POSTFIX") == 0)
		type = postfix;

	else
		type = none;

	//Perform operations dependent on expression type
	if (type == none)
        return 0;

	double answer;

	switch (type)
	{
		case infix:
			answer = calcInfix();
			break;
		case prefix:
			answer = calcPrefix();
			break;
		case postfix:
			answer = calcPostfix();
			break;
	}

	//Display the answer
	cout << "Answer: " << answer << endl;
	system("pause");
	return 0;
}

double calcInfix()
{
	//Display type selection
	cout << "TYPE: INFIX" << endl;

	//Get the expression
	string expression;
	expression = getExpression();

	//Convert to postfix
	expression = infixToPostfix(expression);

	//Calculate and return the answer
	return calcAnswer(expression);
}

double calcPrefix()
{
	//Display type selection
	cout << "TYPE: PREFIX" << endl;

	//Get the expression
	string expression;
	expression = getExpression();

	//Convert to postfix
	expression = prefixToInfix(expression);
	expression = infixToPostfix(expression);

	//Calculate and return the answer
	return calcAnswer(expression);
}

double calcPostfix()
{
	//Display type selection
	cout << "TYPE: POSTFIX" << endl;

	//Get the expression
	string expression;
	expression = getExpression();

	//Calculate and return the answer
	return calcAnswer(expression);
}

string getExpression()
{
	//Get input
	string input;

	cout << "Enter the expression. Put spaces between numbers. " << endl;
	getline(cin, input);
	return input;
}

string infixToPostfix(string infix)
{
	//Definitions
	string postfix;
	stack<char> operators;

	//Read each character of the string
	for (unsigned int i = 0; i < infix.length(); i++)
	{
		//If digit, push into new string
		if (isdigit(infix[i]))
		{
			//If no spaces in expression, throw error
			if (isOperator(infix[i + 1]))
			{
				cout << "Insufficient spacing. Exiting program." << endl;
				system("pause");
				exit(EXIT_FAILURE);
			}

			postfix.push_back(infix[i]);
		}

		//If operator, push to stack
		else if (isOperator(infix[i]))
		{
			//If stack is empty, push operator
			if (operators.empty())
				operators.push(infix[i]);

			//If stack is not empty, check precedence
			else
			{
				//Pop if top operator has greater or equal precedence
				if (precedence(operators.top()) >= precedence(infix[i]))
				{
					while (!operators.empty() &&
						precedence(operators.top()) >= precedence(infix[i]))
					{
						postfix.push_back(operators.top());
						operators.pop();
					}

					//Then push the new operator
					operators.push(infix[i]);
				}

				//Push if precedence is greater than top of stack
				else
					operators.push(infix[i]);
			}
		}

		//Add to string if space or decimal
		else if (infix[i] == SPACE || infix[i] == DECIMAL)
			postfix.push_back(infix[i]);

		//Push if left parenthesis
		else if (infix[i] == LEFT_PAREN)
			operators.push(infix[i]);

		//Pop if right parenthesis
		else if (infix[i] == RIGHT_PAREN)
		{
			//Avoid bad input
			if (operators.empty())
			{
				cout << "Imbalanced parentheses. Ending the program." << endl;
				system("pause");
				exit(EXIT_FAILURE);
			}

			//Pop and store in string until left parenthesis
			while (operators.top() != LEFT_PAREN)
			{
				postfix.push_back(operators.top());
				operators.pop();
			}
			
			//Pop off left parenthesis
			operators.pop();
		}

		//Avoid bad input
		else
		{
			cout << "Invalid input. Ending the program." << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	//Pop any operators still on the stack
	while (!operators.empty())
	{
		postfix.push_back(operators.top());
		operators.pop();
	}

	//Return the postfix expression
	return postfix;
}

string prefixToInfix(string prefix)
{
	stack<string> equations;
	string equation;

	//Reverse the string
	reverse(prefix);

	//Convert to infix
	for (unsigned int i = 0; i < prefix.length(); i++)
	{
		if (isdigit(prefix[i]))
		{
			//Store digit(1-9) in variable
			double number = prefix[i] - 48;

			//Create new iterator for multi-digit check
			unsigned int multiIt = i;

			//Add to variable if multi-digit
			while (isdigit(prefix[++multiIt]))
			{
				number *= 10;
				number += (prefix[multiIt] - 48);
				i++;
			}

			//Check for decimal
			multiIt = i;
			if (prefix[++multiIt] == DECIMAL)
			{
				if (!isdigit(prefix[++multiIt]))
				{
					cout << "Improper use of decimals. "
						<< "Ending the program." << endl;
					system("pause");
					exit(EXIT_FAILURE);
				}

				i++;

				//Assign decimal values to variable
				double decNumber = prefix[multiIt] - 48;
				unsigned int decLength = 1;
				double divisor = 10;

				//Check for multiple digits
				while (isdigit(prefix[++multiIt]))
				{
					decNumber *= 10;
					decNumber += (prefix[multiIt] - 48);
					decLength++;
					i++;
				}

				i++;

				//Add decimal to original number
				for (unsigned int i = decLength; i >= 2; i--)
					divisor *= 10;
				decNumber /= divisor;
				number += decNumber;
			}

			//Push number onto stack
			string temp = to_string(number);
			equations.push(temp);
		}

		if (isOperator(prefix[i]))
		{
			//Set equation equal to top string and pop
			equation = equations.top();
			equations.pop();

			//Insert parentheses if necessary
			if (prefix[i] == '/' || prefix[i] == '*')
			{
				equation.insert(equation.begin(), LEFT_PAREN);
				equation.insert(equation.end(), RIGHT_PAREN);
			}

			//Push spaces and operator into string
			equation.push_back(SPACE);
			equation.push_back(prefix[i]);
			equation.push_back(SPACE);
			
			//Append second operand to right half and push
			string temp = equations.top();
			equations.pop();

			//Insert parentheses if necessary
			if (prefix[i] == '/' || prefix[i] == '*')
			{
				temp.insert(temp.begin(), LEFT_PAREN);
				temp.insert(temp.end(), RIGHT_PAREN);
			}

			equation.append(temp);
			equations.push(equation);
		}
	}

	return equation;
}

double calcAnswer(string postfix)
{
	//Calculate the answer from postfix expression

	//Create stack
	stack<double> numbers;

	//Move numbers and operators into stacks
	for (unsigned int i = 0; i < postfix.length(); i++)
	{
		//If digit, check for multiple digits and decimal
		if (isdigit(postfix[i]))
		{
			//Store digit(1-9) in variable
			double number = postfix[i] - 48;

			//Create new iterator for multi-digit check
			unsigned int multiIt = i;

			//Add to variable if multi-digit
			while (isdigit(postfix[++multiIt]))
			{
				number *= 10;
				number += (postfix[multiIt] - 48);
				i++;
			}

			//Check for decimal
			multiIt = i;
			if (postfix[++multiIt] == DECIMAL)
			{
				if (!isdigit(postfix[++multiIt]))
				{
					cout << "Improper use of decimals. "
						 << "Ending the program." << endl;
					system("pause");
					exit(EXIT_FAILURE);
				}

				i++;

				//Assign decimal values to variable
				double decNumber = postfix[multiIt] - 48;
				unsigned int decLength = 1;
				double divisor = 10;

				//Check for multiple digits
				while (isdigit(postfix[++multiIt]))
				{
					decNumber *= 10;
					decNumber += (postfix[multiIt] - 48);
					decLength++;
					i++;
				}

				i++;

				//Add decimal to original number
				for (unsigned int i = decLength; i >= 2; i--)
					divisor *= 10;
				decNumber /= divisor;
				number += decNumber;
			}

			//Push number onto stack
			numbers.push(number);
		}

		//If operator, pop 2 and perform operation
		else if (isOperator(postfix[i]))
		{
			//Pop top 2 numbers into variables
			double num2 = numbers.top();
			numbers.pop();

			double num1 = numbers.top();
			numbers.pop();

			//Perform operation on variables
			switch (postfix[i])
			{
			case '+':
				num1 += num2;
				break;
			case '-':
				num1 -= num2;
				break;
			case '*':
				num1 *= num2;
				break;
			case '/':
				num1 /= num2;
				break;
			}

			//Push result back onto stack and pop op
			numbers.push(num1);
		}
	}

	//Return the result
	double result = numbers.top();
	numbers.pop();
	return result;
}

void reverse(string &sReverse)
{
	stack<char> symbols;
	int extraDigits = 0;

	//Push all characters onto a stack
	for (unsigned int i = 0; i < sReverse.length(); i++)
	{
		if (isdigit(sReverse[i]))
		{
			int index = i;

			//Reverse to prevent backwards numbers
			while (isdigit(sReverse[index + 1]) || sReverse[index + 1] == DECIMAL)
			{
				extraDigits++;
				index++;
			}

			if (extraDigits > 0)
			{
				for (extraDigits; extraDigits >= 0; extraDigits--)
					symbols.push(sReverse[i + extraDigits]);
				extraDigits++;
			}

			//Push if no reversal needed
			else
				symbols.push(sReverse[i]);

			i = index;
		}

		//Push if operator
		else if (isOperator(sReverse[i]))
			symbols.push(sReverse[i]);

		//Push LEFT_PAREN if RIGHT_PAREN
		else if (sReverse[i] == RIGHT_PAREN)
			symbols.push(LEFT_PAREN);

		//Push RIGHT_PAREN if LEFT_PAREN
		else if (sReverse[i] == LEFT_PAREN)
			symbols.push(RIGHT_PAREN);

		else if (sReverse[i] == SPACE)
			symbols.push(sReverse[i]);

		//Throw error otherwise.
		else
		{
			cout << "Invalid expression. Exiting the program. " << endl;
			system("pause");
			exit(EXIT_FAILURE);
		}
	}

	//Clear the string
	sReverse.clear();

	//Pop off all characters to reverse string
	while (!symbols.empty())
	{
		sReverse.push_back(symbols.top());
		symbols.pop();
	}
}
bool isOperator(char input)
{
	//Determine if input is an operator
	if (input == '+' || input == '-' || input == '/' || input == '*')
		return true;
	else
		return false;
}

int precedence(char input)
{
	//Assign precedence based on input
    if (input == '/' || input == '*')
			return 2;
	else if (input == '+' || input == '-')
		return 1;
	else
		return 0;
}