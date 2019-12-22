// FractEvaluate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
int largetCommonDividor(int a, int b)
{
    if (a == 0 && b == 0) throw invalid_argument("invalid input");
    if (a == 0 || b == 0) return 1;
    if (a == b) return a;
    if (a < 0) return largetCommonDividor(-a, b);
    if (a < b) return largetCommonDividor(b, a);
    if (a % b == 0) return b;
    return largetCommonDividor(b, a % b);
}
void simply(pair<int, int>& p)
{
    int c = largetCommonDividor(p.first, p.second);
    p.first = p.first / c;
    p.second = p.second / c;
}
pair<int, int> fractAdd(pair<int, int>& a, pair<int, int>& b)
{
    pair<int, int> ans;
    ans.first = a.first * b.second + a.second * b.first;
    ans.second = a.second * b.second;
    simply(ans);
    return ans;
}
pair<int, int> fractMinus(pair<int, int>& a, pair<int, int>& b)
{
    pair<int, int> ans;
    ans.first = a.first * b.second - a.second * b.first;
    ans.second = a.second * b.second;
    simply(ans);
    return ans;
}
pair<int, int> fractMultiply(pair<int, int>& a, pair<int, int>& b)
{
    pair<int, int> ans;
    ans.first = a.first * b.first;
    ans.second = a.second * b.second;
    simply(ans);
    return ans;
}
pair<int, int> fractDivide(pair<int, int>& a, pair<int, int>& b)
{
    if (b.first == 0) throw invalid_argument("denominator is 0");
    pair<int, int> ans;
    ans.first = a.first * b.second;
    ans.second = a.second * b.first;
    simply(ans);
    return ans;
}
pair<int, int> parseToken(string& s)
{
    pair<int, int> res;
    int num = 0;
    int sig = (s[0] == '-') ? -1 : 1;
    int k = 0; // integer part
    bool hasIntegerOnly = true;
    int i = s[0] == '-' ? 1 : 0;
    for (; i < s.size(); i++)
    {
        if (s[i] == '/')
        {
            res.first = num;
            num = 0;
            hasIntegerOnly = false;
        }
        else if (s[i] == '_')
        {
            k = num;
            num = 0;
            hasIntegerOnly = false;
        }
        else if (s[i] >= '0' && s[i] <= '9')
        {
            num = num * 10 + (s[i] - '0');
        }
        else
        {
            throw invalid_argument("invalid input character");
        }
    }
    if (hasIntegerOnly)
    {
        res.first = num;
        res.second = 1;
    }
    else
    {
        res.second = num;
    }
    if (res.second == 0) throw invalid_argument("denominator is 0");
    res.first += abs(k) * res.second;
    res.first *= sig;
    return res;
}
string to_string(pair<int, int> p)
{
    int k = p.first / p.second;
    if (p.first % p.second == 0) return to_string(k);
    if (k == 0)
    {
        // do not output 0 integer value
        return to_string(p.first) + '/' + to_string(p.second);
    }
    string ans = to_string(k) + '_';
    if (p.first < 0) p.first *= -1;
    p.first = p.first % p.second;
    simply(p);
    ans += to_string(p.first) + '/' + to_string(p.second);
    return ans;
}
void operating(pair<int, int>& curRes, pair<int, int>& num, char preop)
{
    switch (preop)
    {
    case '+': curRes = fractAdd(curRes, num); break;
    case '-': curRes = fractMinus(curRes, num); break;
    case '*': curRes = fractMultiply(curRes, num); break;
    case '/': curRes = fractDivide(curRes, num); break;
    default: throw invalid_argument("invalid operator");
    }
}
string evaluate(const string& s)
{
    char preop = '+';
    pair<int, int> res(0, 1);
    pair<int, int> curRes(0, 1); // result from substring consisting of * and /
    pair<int, int> num(0, 1);
    stringstream ss(s);
    string token;
    while (ss >> token)
    {
        if (token == "+" || token == "-")
        {
            operating(curRes, num, preop);
            res = fractAdd(res, curRes);
            curRes = { 0, 1 };
            num = { 0, 1 };
            preop = token[0];
        }
        else if (token == "*" || token == "/")
        {
            operating(curRes, num, preop);
            num = { 0, 1 };
            preop = token[0];
        }
        else
        {
            num = parseToken(token);
        }
    }
    operating(curRes, num, preop);
    res = fractAdd(res, curRes);
    return to_string(res);
};
int main()
{
    /* tested cases
    vector<pair<string, string>> s = {
        {"1/2 * 3_3/4", "1_7/8"},
        {" 1/2 * 3_3/4 ", "1_7/8"},
        {"1/2 * 3_3/4 / 1_1/3", "1_13/32"},
        {"2_3/8 + 9/8", "3_1/2"},
        {"0/1", "0"},
        {"2/2", "1"},
        {"4/2", "2"},
        {" -4/3 ", "-1_1/3"},
        {" 4/3 ", "1_1/3"},
        {"0 - 1/3", "-1/3"},
        {"0 - 5/3", "-1_2/3"},
        {"-2_3/8 + 9/8", "-1_1/4"},
        {"1/2 - 2/3", "-1/6"},
        {"0 + 1 - 2", "-1"},
        {"0 + 2 - 2", "0"},
        {"0 + 3/3 - 2/1", "-1"},
    };
    for (auto t : s)
    {
        string res = evaluate(t.first);
        if (t.second != res)
        {
            cout << t.first << endl;
            cout << res << endl;
        }
    }
    */
    while (1)
    {
        cout << "?";
        string in;
        getline(cin, in);
        if (in == "exit") return 0;
        cout << evaluate(in) << endl;
    }
    return 0;
}