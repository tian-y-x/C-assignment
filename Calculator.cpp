#include <iostream>
#include <cstdio>
#include <regex>
#include <cmath>
#include <vector>
#include <stack>
#include <unordered_map>
#define DEBUG
using namespace std;


const int op_kind=14;
int pos;								//��¼��ȡ��ĳһ���ʽ���ĸ�λ��
int flag;
int countVar = -1;
int flagCalVar;
unordered_map <string, int> linkMap={	 //�����������������ŵ���ϵ

	{"+",0},	{"-",1},	{"*",2},	 {"/",3},	{"%",4},
	{"pow",5},	{"!",6},	{"(",7},	 {")",8},   {"sqrt",9},
	{"sin",10}, {"cos",11}, {"ln",12},   {"#",13}

};

string regOpr[op_kind] = { "^[\+]", "^[-]", "^[\*]", "^[/]", "^[%]","^[p][o][w]", "^[!]", "^[\(]", "^[\)]",
						   "^[s][q][r][t]", "^[s][i][n]",    "^[c][o][s]",    "^[l][n]", "^[#]" };

int prioity[op_kind][op_kind] = { /*���ȼ�ͼ��
   |  | -------------------------- - ��ǰ�����---------------------------- |
   |			+	-	*	/	%  pow	!	(	)	sqrt	sin		cos		ln	 #
   |																		*/
 /*|	 + |*/  1,  1,  0,  0,  0,  0,  0, 	0,	1,   0,		 0,		 0,		0,   1,
 //|	   |
 /*|	 - |*/	1,  1,  0,  0,  0,  0,  0, 	0,	1,   0,		 0,		 0,		0,   1,
 //|	   |
 /*ջ    * |*/	1,  1,  1,  1,  1,  0,  0, 	0,	1,   0,		 0,		 0,		0,	 1,
 //|	   |
 /*��    / |*/	1,  1,  1,  1,  1,  0,  0, 	0,	1,   0,		 0,		 0,		0,   1,
 //|	   |
 /*��	 % |*/	1,  1,  1,  1,  1,  0,  0, 	0,	1,   0,		 0,		 0,		0,	 1,
 //|	   |
 /*��	 ^ |*/	1,  1,  1,  1,  1,  1,  0, 	0,	1,   0,		 0,		 0,		0,	 1,
 //|	   |
 /*��    ! |*/	1,  1,  1,  1,  1,  1,  1, 	2,	1,   1,		 1,		 1,		1,	 1,
 //|	   | 
 /*|     ( |*/  0,	0,	0,	0,	0,	0,	0,	0,	3,	 0,		 0,		 0,		0,	 2,
 //|       |
 /*|     ��|*/	2,  2,  2,  2,  2,  2,  2, 	2,	2,   2,		 2,		 2,		2,	 2,
 //|	   |
 /*|  sqrt |*/	1,  1,  1,  1,  1,  1,  1, 	0,	1,   1,		 1,		 1,		1,	 1,
 //|	   |
 /*|   sin |*/	1,  1,  1,  1,  1,  1,  1, 	0,	1,   1,		 1,		 1,		1,	 1,
 //|	   |
 /*|   cos |*/	1,  1,  1,  1,  1,  1,  1, 	0,	1,   1,		 1,		 1,		1,	 1,
 //|	   |
 /*|    ln |*/  1,  1,  1,  1,  1,  1,  1, 	0,	1,   1,		 1,		 1,		1,	 1,
 //|       |
 /*|     # |*/	0,	0,	0,	0,	0,	0,	0,	0,	2,	 0,		 0,		 0,		0,	 3



};

bool isvar(const string& input, vector <string> &var, stack <double> &stackNum,vector <double> value)
{
	regex reg("^([a-z]+[0-9]*)([=]?)", regex::icase);
	smatch match;
	string::const_iterator iter = input.begin() + pos;
	string::const_iterator iterEnd = input.end();
	if (regex_search(iter, iterEnd, match, reg))
	{
		if (match.length())
		{
			//for ();
			for (int i = 0; i < var.size(); i++)
			{
				if (var[i] == match[1].str())
				{
					//cout << "��������:" << var[i] << endl;
					countVar = i;
					stackNum.push(value[countVar]);
					flagCalVar = 1;
					pos += match[1].length();
					return true;
				}
			}
			pos += match.length();
			string V = match[1].str().c_str();
			//cout << V << endl;
			//cout << "������" << V <<"ѹ��������"<< endl;
			var.push_back(V);
			countVar++;
			return true;
		}
		else
			return false;
	}
}
double isdigit(const string &input)
{
	regex reg("^[0-9]*[\.]?[0-9]+");
	smatch match;

	string::const_iterator iter = input.begin()+pos;
	//cout << *iter << endl;
	string::const_iterator iterEnd = input.end();
	
	if (regex_search(iter, iterEnd, match, reg))
	{
		//cout << match[0].str().c_str() << endl;
		//cout << "len" << match.length() << endl;
		pos += match.length();
		//cout <<"sds" <<pos << endl;
		//cout << atof(match[0].str().c_str()) << endl;
		return atof(match[0].str().c_str());
	}
	else
		return INFINITY;
}

int isOperation(const string& input)
{
	//unordered_map<const string, int>::iterator iterm;

	for (int i=0; i< op_kind; i++)
	{
		regex reg(regOpr[i]);
		smatch match;
		//cout << "pos��" << pos;
		string::const_iterator iter = input.begin() + pos;
		string::const_iterator iterEnd = input.end();
		//cout << regOpr[i] << endl;
		
		if (regex_search(iter, iterEnd, match, reg))
		{
			pos += match.length();
			//cout << match[0].str().c_str() << endl;
			return linkMap[match[0].str()];
		}
		
	}
	return -1;
}
double Calculate(int unary_opr, double num)
{
	int fact = 1;
	int n = round(num);
	switch (unary_opr)
	{
	case 6:
		if (n == 0)
			return 1;
		for (int i = 1; i <= n; i++)
			fact *= i;
		return fact;
		break;
	case 9:
		if (num >= 0)
			return sqrt(num);
		else
			cout << "������ӦΪ�Ǹ���" << endl, exit(-1);
		break;
	case 10:
		return sin(num);
		break;
	case 11:
		return cos(num);
		break;
	case 12:
		if (num > 0)
			return log(num);
		else
			cout << "����ʽ����ӦΪ����" << endl, exit(-1);
		break;
	default:
		cout << "calculate1 ����δ֪����" << endl;
		exit(-1);
	}
}
double Calculate(int binary_opr, double num1, double num2)
{
	switch (binary_opr)
	{
	case 0:
		return num1 + num2;
		break;
	case 1:
		return num1 - num2;
		break;
	case 2:
		return num1 * num2;
		break;
	case 3:
		if (num2 == 0)
			cout << "����Ϊ0,�����˳�", exit(-1);
		return num1 / num2;
		break;
	case 4:
		return (int)num1 % (int)num2;
		break;
	default:
		cout << "calculate2 ����δ֪����" << endl;
		exit(-1);
	}
}
void Analyse(const int cur, const int top, stack <int>& stackOpr, stack<double>& stackNum)
{
	
	switch (prioity[top][cur])
	{
	case 0:
		stackOpr.push(cur);
		//cout << "��" << cur << "ѹ��ջ��" << endl;
		flag = 0;
		break;
	case 1: //ջ����������ȼ��ߣ������������ջ
		if (top == 6 || (top >= 9 && top <= 12))	//���ｫ��cos sin ln sqrt��һԪ��������ִ���
		{
			flag = 1;
			double num = stackNum.top();
			//cout << "��" << num << "����" << endl;
			stackNum.pop();
			double m = Calculate(top, num);
			stackOpr.pop();
			//cout << "��" << m << "ѹ��ջ��" << endl;
			stackNum.push(m);
		}
		else										//��Ԫ�����
		{
			flag = 1;
			double num2 = stackNum.top();
			stackNum.pop();
			//cout << "��" << num2 << "����" << endl;
			if (top == 1 && stackOpr.size() == 2&&stackNum.size()==0)
				stackNum.push(0);
			double num1 = stackNum.top();
			stackNum.pop();
			//cout << "��" << num1 << "����" << endl;
			double m = Calculate(top, num1, num2);
			//cout << "������" << stackOpr.top() << "����" << endl;
			stackOpr.pop();
			//cout << "�����" << m << "ѹ��ջ��" << endl;
			stackNum.push(m);
		}
		break;
	case 3:
		//cout << "--====----tanchu" << stackOpr.top() << endl;
		stackOpr.pop();
		flag = 0;
		break;
	case 2:
		//cout << top << cur << endl;
		cout << "���ʽ���Ϸ�,�����˳�" << endl;
		exit(-1);
		
	default:
		cout << "Analyse��������δ֪����" << endl;
		exit(-1);
	}
}
int main()
{
	stack <double> stackNum;		     //������ֵ�ջ
	stack <int> stackOpr;			     //����������ջ
	vector <string> express;			 //��ű��ʽ������
	vector <string> var;				 //��ű���������
	vector <double> value;               //��ű���ֵ������
	int expCount = 0;					 //���ڼ�¼���ʽ����
	while (1)
	{
		if (expCount % 10 == 0)
			express.resize(10 + expCount);
		if (cin >> express[expCount])
		{
			express[expCount] += "#";
			expCount++;
		}
		else break;
	}
	/*for (int i = 0;i < expCount;i++)
		cout << express[i] << endl;*/
	for (int t = 0; t < expCount; t++)
	{
		//����ÿ�б��ʽ
		//DeleteSpace(express[t]);
		int flagVar = 0;
		//int stackNumLastSize = 0;
		int len = express[t].length();
		pos = 0;
		//cout << "��#ѹ��ջ��" << endl;
		stackOpr.push(op_kind-1);
		int tempOpr = op_kind-1;  //�������ڱ���#��
		while (!stackOpr.empty())
		{         
			double tempNum;
			if (1) 
			{
				if (isvar(express[t], var,stackNum,value))
				{
					flagVar = 1;
				}
				if ((tempOpr!=8)&&(tempNum = isdigit(express[t]))!=INFINITY)
				{
					//cout << "��tempNum" << tempNum << "ѹ��ջ��" << endl;
					stackNum.push(tempNum);
				}
				if ((flag == 1) || ((tempOpr = isOperation(express[t])) != -1))
				{
					Analyse(tempOpr, stackOpr.top(), stackOpr, stackNum);
				}
			}                                                                                                                                                      
			else
				//cout <<"flag=" <<flag << endl;
				cout << "���ʽ���Ϸ��� �����˳� " << endl, exit(-1);
		}
		if (flagVar && flagCalVar == 0)
		{
			value.push_back(stackNum.top());
			cout << var[countVar] << "=" << value[countVar];
			stackNum.pop();
			
			
		}
		else
		{
			cout << "��" << t + 1 << "�б��ʽΪ" << endl;
			cout << stackNum.top() << endl;
			stackNum.pop();
		}
#ifndef DEBUG
		if (stackNum.size() == 0 && stackOpr.size() == 0)
			cout << "��" << t + 1 << "�б��ʽ��������" << endl;
#endif // !1
	}
	
	return 0;
}
