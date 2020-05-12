#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <windows.h>
#include<limits>
#include <ctime>
#include <iomanip>

using namespace std;

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

int P1score = 0, P2score = 0, mode = 1, p1w = 0, p2w = 0, streak = 0, P1score_tot = 0, P2score_tot = 0;
int pt_dec = 0, pt_strk = 0, pt_count = 0, pt_vectorsize = 0, pt_bestresult = 0, pt_worstresult = 100000000, ncont = 0;
int turn = 1, points = 0, plhole, numbpieces, gamecount = 0, cmove = 1, text_color = 15, board_color = 15, gamecount_tot = 0;
bool game_end = false, gameover = false, game_streak = false, repeatcolors = false, perfectturing = false, customnick = false;
string p2 = "Player 2 :", caps_p2 = "PLAYER 2", p1 = "Player 1 :", caps_p1 = "PLAYER 1", newp1 = p1, newp2 = p2;
vector <int> board(12, 4);
vector <int> pt_comp;
float iacoef = 0.5, pt_bestcoef = 0.5;


void DrawLogo()
{
    cout << endl << endl << endl;
    cout << "  |||||   ||    ||  ||||||   || " << endl;
    cout << " ||   ||  ||    ||  ||   ||  || " << endl;
    cout << " ||   ||  ||    ||  ||   ||  || " << endl;
    cout << " ||   ||  ||    ||  ||||||   || " << endl;
    cout << " ||   ||  ||    ||  ||   ||  || " << endl;
    cout << "  |||||   ||||||    ||   ||  || " << "      by Jose' Pedro Peixoto Ferreira"  << endl << endl;
}

void SetColor(unsigned int color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon, color);
}

void ListColors()
{
    cout << "Color List: " << endl<< endl;
    cout << "Black -> 0" << endl << "Blue -> 1" << endl  << "Green -> 2" << endl;
    cout << "Cyan -> 3" << endl << "Red -> 4" << endl  << "Magenta -> 5" << endl;
    cout << "Brown -> 6" << endl << "Light Gray -> 7" << endl  << "Dark Gray -> 8" << endl;
    cout << "Light Blue -> 9" << endl << "Light Green -> 10" << endl  << "Light Cyan -> 11" << endl;
    cout << "Light Red -> 12" << endl << "Light Magenta -> 13" << endl  << "Yellow -> 14" << endl << "White -> 15" << endl;
}

void Retry()
{
    P1score = P2score = points = 0;
    p1 = "Player 1 :";
    caps_p1 = "PLAYER 1";
    turn = cmove = 1;
    board = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    plhole = 0;
    game_end = false;
    DrawLogo();
}

void DrawRules()
{
    cout << endl << "Rules : " << endl << endl << "Ouri is a board game that belongs to the larger family of mancala.";
    cout << endl << "The game requires an ouri board and 48 seeds. ";
    cout << endl << "A typical ouri board has two straight rows of six pits, called \"houses\", "
                    "and optionally one large \"score\" house at either end. ";
    cout << endl << "Each player controls the six houses on their side of the board, and the score house on their end.";
    cout << endl << "The game begins with four seeds in each of the twelve smaller houses.";
    cout << endl << "Objective" << endl << "The objective of the game is to capture more seeds than one's opponent. ";
    cout << endl << "Since the game has only 48 seeds, capturing 25 is sufficient to win the game. ";
    cout << endl << "Since there is an even number of seeds, it is possible for the game to end in a draw, where each player has captured 24.";
    cout << endl << "Sowing" << endl << "Players take turns moving the seeds. ";
    cout << endl << "On a turn, a player chooses one of the six houses under their control. ";
    cout << endl << "The player removes all seeds from that house, and distributes them, "
                    "dropping one in each house counter-clockwise from this house, in a process called sowing. ";
    cout << endl << "Seeds are not distributed into the end scoring houses, nor into the house drawn from. ";
    cout << endl << "The starting house is always left empty, if it contained 12 (or more) seeds, it is skipped, and the twelfth seed is placed in the next house.";
    cout << endl << endl << "Capturing" << endl << "In this version of Ouri, "
                                                   "capturing occurs only when a player brings the count of an opponent's house to exactly two or three with the final seed he sowed in that turn. ";
    cout << endl << "This always captures the seeds in the corresponding house, and possibly more:";
    cout << endl << "If the previous-to-last seed also brought an opponent's house to two or three, "
                    "these are captured as well, and so on until a house is reached which does not contain two or three seeds or does not belong to the opponent. ";
    cout << endl << "The captured seeds are placed in the player's scoring house (or set aside if the board has no scoring houses). ";
    cout << endl << "However, if a move would capture all of an opponent's seeds, the capture is forfeited since this would prevent the opponent from continuing the game, and the seeds are instead left on the board. ";
    cout << endl << endl << "Let the Opponent Play" << endl << "The proscription against capturing all an opponent's seeds is related to a more general idea, "
                                                               "that one ought to make a move that allows the opponent to continue playing. ";
    cout << endl << "If an opponent's houses are all empty, the current player must make a move that gives the opponent seeds.";
    cout << endl << "If no such move is possible, the current player captures all seeds in his/her own territory, ending the game.";
    cout << endl << endl << "NOTE: The CPU's will always follow this set of rules, forcing endgame only when there is no other option or the game is stuck on an infinite loop.";
    cout << endl << endl << "Have fun!";
}

string NickName(string aname)
{
    string newname;
    while(1)
    {
        cout << endl << "Insert your Nickname, " << aname;
        cin >> newname;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "That wasn't a valid nickname." << endl;
        }
        else
            break;
    }
    return newname;
}

void Move(int hole, vector <int> &a_board)
{
    int inithole = hole;
    int npieces = a_board[hole];
    a_board[hole] = 0;
    for (npieces; npieces > 0; npieces--)
    {
        hole++;
        hole = hole % 12;
        if (hole != inithole)
            a_board[hole] += 1;
        else
            npieces += 1;
    }
}

int Catch(int hole, int nhole, vector <int> &a_board)
{
    int lhole = hole + nhole;
    points = 0;
    for (int lasthole = (lhole + 24) % 12; a_board[(lasthole + 24) % 12] == 2 || a_board[(lasthole + 24) % 12] == 3; lasthole--)
    {
        lasthole = (lasthole + 24) % 12;
        points += a_board[lasthole];
        a_board[lasthole] = 0;
    }
    return points;
}

bool ValidMove(int hole)
{
    if (game_end)
        return true;
    int sum = 0;
    int otherturn = (turn % 2) + 1;
    vector <int> copyboard(12);
    copyboard = board;
    Move(hole, copyboard);
    Catch(hole, board[hole], copyboard);
    for (int i = 0; i < 6 ; i++)
        sum += copyboard[(otherturn - 1) * 6 + i];
    if (sum == 0)
        return false;
    else
        return true;
}

void DrawBoard(vector <string> aboard, unsigned int acolor)
{
    unsigned int ccolor = acolor;
    SetColor(ccolor);
    cout << endl << "  ";
    for (int i = 11; i > 5; i--)
    {
        if ((board[i] == 0 || !ValidMove(i)) && turn == 2)
            ccolor = 4;
        SetColor(ccolor);
        if (i + 1 < 10)
            cout << "H0" << to_string(i+1) << "   ";
        else
            cout << "H" << to_string(i+1) << "   ";
        ccolor = acolor;
        SetColor(ccolor);
    }
    cout << endl;
    cout << " _____ _____ _____ _____ _____ _____ " << endl;
    cout << "|     |     |     |     |     |     |" << endl;
    cout << "| ";
    for (int i = 11; i > 5; i--)
    {
        if ((board[i] == 0 || !ValidMove(i)) && turn == 2)
            ccolor = 4;
        SetColor(ccolor);
        cout << aboard[i];
        ccolor = acolor;
        SetColor(ccolor);
        cout << "  | ";
    }
    cout << "   -->    " << caps_p2 << " SIDE";
    ccolor = acolor;
    SetColor(ccolor);
    cout << endl << "|_____|_____|_____|_____|_____|_____|" << endl;
    cout << endl << endl << "  ";
    for ( int i = 0; i < 6; i++)
    {
        if ((board[i] == 0 || !ValidMove(i)) && turn == 1)
            ccolor = 4;
        SetColor(ccolor);
        cout << "H0" << to_string(i+1) << "   ";
        ccolor = acolor;
        SetColor(ccolor);
    }
    cout << endl;
    cout << " _____ _____ _____ _____ _____ _____ " << endl;
    cout << "|     |     |     |     |     |     |";
    cout << endl << "| ";
    for (int i = 0; i < 6; i++)
    {
        if ((board[i] == 0 || !ValidMove(i)) && turn == 1)
            ccolor = 4;
        SetColor(ccolor);
        cout << aboard[i];
        ccolor = acolor;
        SetColor(ccolor);
        cout << "  | ";
    }
    cout << "   -->    " << caps_p1 << " SIDE";
    ccolor = acolor;
    SetColor(ccolor);
    cout << endl << "|_____|_____|_____|_____|_____|_____|" << endl;
    cout << endl;
}

int PlayerTurn (int hole)
{
    Move(hole, board);
    return Catch(hole, numbpieces, board);
}

int RandomPlayer()
{
    vector <int> possibilities;
    if (mode == 6 || mode == 7)
        possibilities = {1, 2, 3, 4, 5, 6};
    else if (mode == 8 && turn == 1)
        possibilities = {1, 2, 3, 4, 5, 6};
    else
        possibilities = {7, 8, 9, 10, 11, 12};
    vector <int> validp;
    for (int i = 0; i < possibilities.size(); i++)
    {
        if (ValidMove(possibilities[i] - 1) && board[possibilities[i] - 1] != 0)
            validp.push_back(possibilities[i]);
    }
    if (validp.size() == 0 || (board[1] + board[2] + board[3] + board[4] + board[7] + board[8] + board[9] + board[10] == 0 && board[5] == 1 && board[11] == 1))
        return 100;
    else
    {
        int choice = rand() % validp.size();
        return validp[choice];
    }
}

vector <int> Simulation_Outcome(vector <int> vpossibilities, vector <int> aboard)
{
    vector <int> copyboard = aboard;
    vector <int> outcome(6, 0);
    vector <int> best_plays;
    int numbpieces = 0;
    for (int i = 0; i < vpossibilities.size(); i++)
    {
        if (ValidMove(vpossibilities[i] - 1) == false || copyboard[vpossibilities[i] - 1] == 0)
            outcome[i] = -1;
        else
        {
            numbpieces = copyboard[vpossibilities[i] - 1];
            Move(vpossibilities[i] - 1, copyboard);
            outcome[i] = Catch(vpossibilities[i] - 1, numbpieces, copyboard);
        }
        copyboard = aboard;
        if (mode == 3 || (mode == 5 && turn == 1) || mode == 7 || mode == 9)
            cout << "Hole: " << i + 1 + 6 * (turn - 1) << "   Outcome: " << outcome[i] << endl;
    }
    return outcome;
}

vector <int> Simulation_Board(vector <int> aboard, int themove)
{
    int numbpieces = 0;
    numbpieces = aboard[themove];
    Move(themove, aboard);
    Catch(themove, numbpieces, aboard);
    return aboard;
}

int FindBest(vector <int> ploutcome)
{
    int best = -2;
    for (int i = 0; i < ploutcome.size(); i++)
    {
        if (ploutcome[i] > best)
            best = ploutcome[i];
    }
    return best;
}

float FindBestF(vector <float> ploutcome)
{
    float best = -200.0;
    for (int i = 0; i < ploutcome.size(); i++)
    {
        if (ploutcome[i] > best)
            best = ploutcome[i];
    }
    return best;
}

int IAPlayer()
{
    vector <int> possibilities;
    if (mode == 5 || (mode == 9 && turn == 1))
        possibilities = {1, 2, 3, 4, 5, 6};
    else
        possibilities = {7, 8, 9, 10, 11, 12};
    vector <int> outcome = Simulation_Outcome(possibilities, board);
    vector <int> best_plays;
    int bestp = FindBest(outcome);
    for (int i = 0; i < outcome.size(); i++)
    {
        if (outcome[i] == bestp)
            best_plays.push_back(i);
    }
    if (bestp == -1 || (board[1] + board[2] + board[3] + board[4] + board[7] + board[8] + board[9] + board[10] == 0 && board[5] == 1 && board[11] == 1))
        return 100;
    else if (best_plays.size() == 1)
        return possibilities[best_plays[0]];
    else
    {
        int choice = rand() % best_plays.size();
        return possibilities[best_plays[choice]];
    }
}

vector <int> NextPlay_Outcome()
{
    vector <int> plpossibilities;
    vector <int> iapossibilities;
    if (mode == 10 && turn == 1)
    {
        plpossibilities = {7, 8, 9, 10, 11, 12};
        iapossibilities = {1, 2, 3, 4, 5, 6};
    }
    else
    {
        plpossibilities = {1, 2, 3, 4, 5, 6};
        iapossibilities = {7, 8, 9, 10, 11, 12};
    }
    vector <int> bploutcome(6, -2);
    vector <int> ploutcome;
    for (int i = 0; i < iapossibilities.size(); i++)
    {
        if (ValidMove(iapossibilities[i] - 1))
        {
            vector <int> iaboard = Simulation_Board(board, iapossibilities[i] - 1);
            ploutcome = Simulation_Outcome(plpossibilities, iaboard);
            int casebest = FindBest(ploutcome);
            bploutcome[i] = casebest;
        }
    }
    return bploutcome;
}

int ComplexIA(float coef)
{
    vector <int> iapossibilities;
    if (mode == 10 && turn == 1)
        iapossibilities = {1, 2, 3, 4, 5, 6};
    else
        iapossibilities = {7, 8, 9, 10, 11, 12};
    vector <int> selfoutc = Simulation_Outcome(iapossibilities, board);
    vector <int> advoutc = NextPlay_Outcome();
    vector <int> best_plays;
    vector <float> averageout;
    for (int i = 0; i < selfoutc.size(); i++)
    {
        if (selfoutc[i] != -1)
        {
            float avg = selfoutc[i] - coef * advoutc[i];
            cout << "Hole: " << i + 1 + (turn - 1) * 6 << "   Compound Outcome: " << avg << endl;
            averageout.push_back(avg);
        }
        else
        {
            cout << "Hole: " << i + 1 + (turn - 1) * 6 << "   Compound Outcome: " << -100 << endl;
            averageout.push_back(-100);
        }
    }
    float bestpp = FindBestF(averageout);
    for (int i = 0; i < averageout.size(); i++)
    {
        if (averageout[i] == bestpp)
            best_plays.push_back(i);
    }
    if (bestpp == -100 || (board[1] + board[2] + board[3] + board[4] + board[7] + board[8] + board[9] + board[10] == 0 && board[5] == 1 && board[11] == 1))
        return 100;
    else if (best_plays.size() == 1)
        return iapossibilities[best_plays[0]];
    else
    {
        int choice = rand() % best_plays.size();
        return iapossibilities[best_plays[choice]];
    }
}


int main()
{
    SetColor(text_color);
    DrawLogo();
    int sum = 0;
    while (!gameover && mode != 0)
    {
        if (!game_streak && !perfectturing)
        {
            cout << "Mode 1: Player vs Player" << endl;
            cout << "Mode 2: Player vs Random CPU" << endl;
            cout << "Mode 3: Player vs Normal IA CPU" << endl;
            cout << "Mode 4: Player vs Turing IA CPU" << endl;
            cout << "Mode 5: CPU BATTLE --> Turing IA CPU vs Normal IA CPU" << endl;
            cout << "Mode 6: CPU BATTLE --> Turing IA CPU vs Random CPU" << endl;
            cout << "Mode 7: CPU BATTLE --> Normal IA CPU vs Random CPU" << endl;
            cout << "Mode 8: CPU BATTLE --> Random CPU vs Random CPU" << endl;
            cout << "Mode 9: CPU BATTLE --> Normal IA CPU vs Normal IA CPU" << endl;
            cout << "Mode 10: CPU BATTLE --> Turing IA CPU vs Turing IA CPU" << endl;
            cout << "Select Mode (1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 || 100 to read the rules || 1000 to edit Turing IA || 0 to exit) : ";
            while (1)
            {
                if (perfectturing)
                    break;
                cin >> mode;
                if ((cin.fail() || mode > 10 || mode < 0) && mode != 100 && mode != 1000)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Input a valid mode (0 -> 10)." << endl;
                    cout << "Select Mode (1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 || 100 to read the rules || 1000 to edit Turing IA || 0 to exit) : ";
                }
                else if (mode == 100)
                {
                    DrawRules();
                    cout << endl << "Select Mode (1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 || 100 to read the rules || 1000 to edit Turing IA || 0 to exit) : ";
                }
                else if (mode == 1000)
                {
                    cout << "How much importance do you want the Turing IA to give to the opponent's moves? " << endl;
                    cout << "Input a float between 0 and 2 to modify the Turing IA || Input 100 to perfect the Turing IA || Any other number to return to default : ";
                    while (1)
                    {
                        cin >> iacoef;
                        if (cin.fail())
                        {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "That's not a valid number!" << endl;
                            cout << "How much importance do you want the Turing IA to give to the opponent's moves? " << endl;
                            cout << "Input a float between 0 and 2 to modify the Turing IA || Input 100 to perfect the Turing IA || Any other number to return to default : ";
                        }
                        else if ((iacoef < 0 || iacoef > 2) && iacoef != 100)
                        {
                            iacoef = pt_bestcoef;
                            cout << "Turing Coeffecient set to " << iacoef << endl;
                            break;
                        }
                        else if (iacoef == 100)
                        {
                            perfectturing = true;
                            cout << "Perfect Turing Protocol Initiated" << endl;
                            pt_bestresult = 0;
                            pt_count = 0;
                            pt_comp.clear();
                            cout << "Introduce the number of decimal places: ";
                            while (1)
                            {
                                cin >> pt_dec;
                                if (cin.fail() || pt_dec < 0)
                                {
                                    cin.clear();
                                    cin.ignore(1000, '\n');
                                    cout << "That wasn't a valid number (Any positive integer)." << endl;
                                    cout << "Introduce the number of decimal places: ";
                                }
                                else
                                    break;
                            }
                            cout << "Introduce the number of games of each streak: ";
                            while (1)
                            {
                                cin >> pt_strk;
                                if (cin.fail() || pt_strk < 0)
                                {
                                    cin.clear();
                                    cin.ignore(1000, '\n');
                                    cout << "That wasn't a valid number (Any positive integer)." << endl;
                                    cout << "Introduce the number of games of each streak: ";
                                }
                                else
                                    break;
                            }
                            pt_worstresult = 3 * pt_strk;
                            break;
                        }
                        else
                        {
                            cout << "Turing Coeffecient set to " << iacoef << endl;
                            break;
                        }
                    }
                    if (!perfectturing)
                        cout << endl << "Select Mode (1, 2, 3, 4, 5, 6, 7, 8, 9 or 10 || 100 to read the rules || 1000 to edit Turing IA || 0 to exit) : ";
                }
                else
                    break;
            }
        }
        else
        {
            cout << endl << "GAME NUMBER : " << gamecount << endl;
            if (perfectturing)
                cout << "Turing Coeffecient Under Test: " << iacoef << endl;
        }
        if (perfectturing)
        {
            mode = 5;
            if (streak == 0)
                streak = pt_strk;
            pt_vectorsize = pow(10, pt_dec) * 2 + 1;
            iacoef = pt_count * pow(10, -pt_dec);
        }
        if (mode >= 5 && streak == 0 && !perfectturing)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Game Streak (0 for no game streak || Any other positive integer for the size of the streak || Negative integers for <<Don't ask again>>) : ";
            while (1)
            {
                cin >> streak;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Input a valid streak (Any integer)" << endl;
                    cout << "Game Streak (0 for no game streak || Any other positive integer for the size of the streak || Negative integers for <<Don't ask again>>) : ";
                }
                else
                    break;
            }
        }
        if (streak > 0)
            game_streak = true;
        if (!repeatcolors && mode != 0 && !perfectturing && (streak <= 0 || (streak > 0 && gamecount == 0)))
        {
            int rcolors = 0;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Do you want to keep the same colors? (1 for Yes [or] 0 for No) : ";
            while (1)
            {
                cin >> rcolors;
                if (cin.fail() || rcolors > 1 || rcolors < 0)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Input a valid answer (0 or 1)." << endl;
                    cout << "Do you want to keep the same colors? (1 for Yes [or] 0 for No) : ";
                }
                else
                    break;
            }
            if (rcolors == 0)
                repeatcolors = false;
            else
                repeatcolors = true;
        }
        if (!repeatcolors && mode != 0 && !perfectturing && (streak <= 0 || gamecount == 0))
        {
            ListColors();
            cout << "Choose the Text UI Color (from the list above) : ";
            cin.clear();
            cin.ignore(1000, '\n');
            while(1)
            {
                cin >> text_color;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    ListColors();
                    cout << "Input a valid answer (0 -> 15)." << endl;
                    cout << "Choose the Text UI Color (from the list above) : ";
                }
                else if (text_color < 0 || text_color > 15)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    ListColors();
                    cout << "Number not on the list!" << endl;
                    cout << "Choose the Text UI Color (from the list above) : ";
                }
                else
                    break;
            }
            ListColors();
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Choose the Board Color (from the list above) : ";
            while (1)
            {
                cin >> board_color;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    ListColors();
                    cout << "Input a valid answer (0 -> 15)." << endl;
                    cout << "Choose the Board Color (from the list above) : ";
                }
                else if (board_color < 0 || board_color > 15)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    ListColors();
                    cout << "Number not on the list!" << endl;
                    cout << "Choose the Board Color (from the list above) : ";
                }
                else
                    break;
            }
        }
        if (mode < 5)
        {
            int cnames = 2;
            while(1)
            {
                cout << endl << "Do you want to customize Player Nicknames? (1 for Yes [or] 0 for No) :";
                cin >> cnames;
                if (cin.fail() || cnames < 0 || cnames > 1)
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "That wasn't a valid answer." << endl;
                }
                else
                    break;
            }
            if (cnames == 1)
                customnick = true;
        }
        while (!game_end && plhole != 100 && mode != 0)
        {
            for (int i = 0; i < 6; i++)
                sum += board[(turn - 1) * 6 + i];
            if (sum == 0)
                game_end = true;
            if (ncont == 0)
            {
                switch (mode)
                {
                    case 0:
                    {
                        game_end = true;
                        gameover = true;
                        break;
                    }
                    case 1:
                    {
                        p2 = "Player 2 :";
                        caps_p2 = "PLAYER 2";
                        if (customnick) {
                            newp1 = NickName(p1);
                            p1 = newp1 + " :";
                            caps_p1 = newp1;
                            for (int i = 0; i < size(caps_p1); i++)
                                caps_p1[i] = toupper(caps_p1[i]);
                            newp2 = NickName(p2);
                            p2 = newp2 + " :";
                            caps_p2 = newp2;
                            for (int i = 0; i < size(caps_p2); i++)
                                caps_p2[i] = toupper(caps_p2[i]);
                        }
                        ncont ++;
                        break;
                    }
                    case 2:
                    {
                        p2 = "Random CPU :";
                        caps_p2 = "RANDOM CPU";
                        if (customnick)
                        {
                            newp1 = NickName(p1);
                            p1 = newp1 + " :";
                            caps_p1 = newp1;
                            for (int i = 0; i < size(caps_p1); i++)
                                caps_p1[i] = toupper(caps_p1[i]);
                        }
                        ncont ++;
                        break;
                    }
                    case 3:
                    {
                        p2 = "Normal IA CPU :";
                        caps_p2 = "NORMAL IA CPU";
                        if (customnick)
                        {
                            newp1 = NickName(p1);
                            p1 = newp1 + " :";
                            caps_p1 = newp1;
                            for (int i = 0; i < size(caps_p1); i++)
                                caps_p1[i] = toupper(caps_p1[i]);
                        }
                        ncont ++;
                        break;
                    }
                    case 4:
                    {
                        p2 = "Turing IA CPU :";
                        caps_p2 = "TURING IA CPU";
                        if (customnick)
                        {
                            newp1 = NickName(p1);
                            p1 = newp1 + " :";
                            caps_p1 = newp1;
                            for (int i = 0; i < size(caps_p1); i++)
                                caps_p1[i] = toupper(caps_p1[i]);
                        }
                        ncont ++;
                        break;
                    }
                    case 5:
                    {
                        p1 = "Normal IA CPU :";
                        caps_p1 = "NORMAL IA CPU";
                        p2 = "Turing IA CPU :";
                        caps_p2 = "TURING IA CPU";
                        break;
                    }
                    case 6:
                    {
                        p1 = "Random CPU :";
                        caps_p1 = "RANDOM CPU";
                        p2 = "Turing IA CPU :";
                        caps_p2 = "TURING IA CPU";
                        break;
                    }
                    case 7:
                    {
                        p1 = "Random CPU :";
                        caps_p1 = "RANDOM CPU";
                        p2 = "Normal IA CPU :";
                        caps_p2 = "NORMAL IA CPU";
                        break;
                    }
                    case 8:
                    {
                        p1 = "Random CPU 1 :";
                        caps_p1 = "RANDOM CPU 1";
                        p2 = "Random CPU 2 :";
                        caps_p2 = "RANDOM CPU 2";
                        break;
                    }
                    case 9:
                    {
                        p1 = "Normal IA CPU 1 :";
                        caps_p1 = "NORMAL IA CPU 1";
                        p2 = "Normal IA CPU 2 :";
                        caps_p2 = "NORMAL IA CPU 2";
                        break;
                    }
                    case 10:
                    {
                        p1 = "Turing IA CPU 1 :";
                        caps_p1 = "TURING IA CPU 1";
                        p2 = "Turing IA CPU 2 :";
                        caps_p2 = "TURING IA CPU 2";
                        break;
                    }
                }
            }
            SetColor(text_color);
            cout << "MOVE: " << cmove << endl;
            cout << "------------SCORE------------" << endl;
            cout << p1 << P1score << "  vs  " << p2 << P2score << endl;
            cout << "------------BOARD------------" << endl;
            vector <string> sboard(12, "0");
            for (int i = 0; i < board.size(); i++)
            {
                if (board[i] < 10)
                    sboard[i] = " " + to_string(board[i]);
                else
                    sboard[i] = to_string(board[i]);
            }
            DrawBoard(sboard, board_color);
            switch (turn)
            {
                case 1:
                {
                    cout << caps_p1  << " TURN" << endl;
                    cout << newp1 << " choose the hole (1 --> 6 or 100 to force endgame): ";
                    if (mode > 0 && mode < 5)
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        while (1)
                        {
                            cin >> plhole;
                            if ((cin.fail() || plhole < 1 || plhole > 6 || board[plhole - 1] == 0 || ValidMove(plhole - 1) == false) && plhole != 100)
                            {
                                cin.clear();
                                cin.ignore(1000, '\n');
                                cout << "Please choose a Valid Move! ";
                                if (plhole >= 1 && plhole <= 6)
                                    if (board[plhole - 1] != 0)
                                        cout << "(One that leaves a moving option to the opponent)";
                                    else
                                        cout << "(Choose an hole with at least 1 piece)";
                                cout << endl << newp1 << " choose the hole (1 --> 6 or 100 to force endgame): ";
                            }
                            else if (plhole == 100)
                            {
                                game_end = true;
                                break;
                            }
                            else
                                break;
                        }
                    }
                    else if (mode == 5 || mode == 9)
                    {
                        cout << endl;
                        int iachoice = IAPlayer();
                        if (mode == 5)
                            cout << "IA CPU choice: " << iachoice << endl;
                        else
                            cout << "IA CPU 1 choice: " << iachoice << endl;
                        plhole = iachoice;
                    }
                    else if (mode == 6 || mode == 7 || mode == 8)
                    {
                        cout << endl;
                        int iachoice = RandomPlayer();
                        if (mode == 8)
                            cout << "Random CPU 1 choice: " << iachoice << endl;
                        else
                            cout << "Random CPU choice: " << iachoice << endl;
                        plhole = iachoice;
                    }
                    else if (mode == 10)
                    {
                        cout << endl;
                        int iachoice = ComplexIA(iacoef);
                        cout << "Turing IA CPU 1 choice: " << iachoice << endl;
                        plhole = iachoice;
                    }
                    if (plhole == 100)
                    {
                        game_end = true;
                        break;
                    }
                    if (plhole < 13)
                    {
                        numbpieces = board[plhole - 1];
                    }
                    P1score += PlayerTurn(plhole - 1);
                    turn = 2;
                    break;
                }
                case 2:
                {
                    cout << caps_p2 << " TURN" << endl;
                    switch (mode)
                    {
                        case 1:
                        {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << newp2 << " choose the hole (7 --> 12 or 100 to force endgame): ";
                            while (1)
                            {
                                cin >> plhole;
                                if ((cin.fail() || plhole < 7 || plhole > 12 || board[plhole - 1] == 0 || ValidMove(plhole - 1) == false) && plhole != 100)
                                {
                                    cin.clear();
                                    cin.ignore(1000, '\n');
                                    cout << "Please choose a Valid Move! ";
                                    if (plhole >= 7 && plhole <= 12)
                                        if (board[plhole - 1] != 0)
                                            cout << "(One that leaves a moving option to the opponent)";
                                        else
                                            cout << "(Choose an hole with at least 1 piece)";
                                    cout << endl << newp2 << " choose the hole (7 --> 12 or 100 to force endgame): ";
                                }
                                else if (plhole == 100)
                                {
                                    game_end = true;
                                    break;
                                }
                                else
                                    break;
                            }
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }

                        case 2:
                        {
                            int rpchoice = RandomPlayer();
                            cout << "Random CPU choice: " << rpchoice << endl;
                            plhole = rpchoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                        case 3:
                        {
                            int iachoice = IAPlayer();
                            cout << "IA CPU choice: " << iachoice << endl;
                            plhole = iachoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                        case 4:
                        {
                            int ciachoice = ComplexIA(iacoef);
                            cout << "Turing IA CPU choice: " << ciachoice << endl;
                            plhole = ciachoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                        case 5:
                        {
                            int ciachoice = ComplexIA(iacoef);
                            cout << "Turing IA CPU choice: " << ciachoice << endl;
                            plhole = ciachoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                        case 6:
                        {
                            int ciachoice = ComplexIA(iacoef);
                            cout << "Turing IA CPU choice: " << ciachoice << endl;
                            plhole = ciachoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                        case 7:
                        {
                            int iachoice = IAPlayer();
                            cout << "IA CPU choice: " << iachoice << endl;
                            plhole = iachoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                        case 8:
                        {
                            int rpchoice = RandomPlayer();
                            cout << "Random CPU 2 choice: " << rpchoice << endl;
                            plhole = rpchoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                        case 9:
                        {
                            int iachoice = IAPlayer();
                            cout << "IA CPU 2 choice: " << iachoice << endl;
                            plhole = iachoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                        case 10:
                        {
                            int ciachoice = ComplexIA(iacoef);
                            cout << "Turing IA CPU 2 choice: " << ciachoice << endl;
                            plhole = ciachoice;
                            if (plhole < 13)
                            {
                                numbpieces = board[plhole - 1];
                                P2score += PlayerTurn(plhole - 1);
                            }
                            turn = 1;
                            break;
                        }
                    }
                    if (plhole == 100)
                    {
                        game_end = true;
                        break;
                    }
                    break;
                }
            }
            cmove ++;
            cout << "-----------------------------" << endl << endl << "-----------------------------" << endl << endl << endl;
        }
        if (mode != 0)
        {
            for (int i = 0; i < 6; i++)
                P1score += board[i];
            for (int i = 0; i < 6; i++)
                P2score += board[6 + i];
            SetColor(text_color);
            cout << " ------------GAME   OVER------------" << endl;
            cout << " ------------FINAL SCORE------------" << endl;
            cout << p1 << P1score << "  vs  " << p2 << P2score << endl;
            if (P1score == P2score)
            {
                gamecount_tot ++;
                if (game_streak)
                {
                    P1score_tot += P1score;
                    P2score_tot += P2score;
                    gamecount += 1;
                }
                cout << "------------DRAW------------" << endl;
            }
            else if (P1score > P2score)
            {
                gamecount_tot ++;
                if (game_streak)
                {
                    P1score_tot += P1score;
                    P2score_tot += P2score;
                    gamecount += 1;
                    p1w += 1;
                }
                cout << "------------" << caps_p1 << " WINS!!!------------" << endl;
            }
            else if (P1score < P2score)
            {
                gamecount_tot ++;
                if (game_streak)
                {
                    P1score_tot += P1score;
                    P2score_tot += P2score;
                    gamecount += 1;
                    p2w += 1;
                }
                cout << "------------" << caps_p2 << " WINS!!!------------" << endl;
            }
            if (gamecount == streak && game_streak)
            {
                cout << endl << endl << endl << "END OF STREAK" << endl;
                if (perfectturing)
                    cout << "End of Testing for Turing Coeffecient: " << iacoef << endl;
                cout << "There were played " << gamecount << " games." << endl;
                cout << "RESULT ---> " << p1 << " " << p1w << "  vs  " << p2 << " " << p2w << "  |  Number of draws: "
                     << gamecount - (p1w + p2w) << endl;
                cout << "RESULT(%) ---> " << p1 << " " << (float(p1w) / float(gamecount)) * 100 << "  vs  ";
                cout << p2 << " " << (float(p2w) / float(gamecount)) * 100 << "  |  Number of draws: "
                     << (float((gamecount) - (p1w + p2w)) / float(gamecount)) * 100 << endl;
                cout << "TOTAL POINTS ---> " << p1 << " " << P1score_tot << "  vs  " << p2 << " " << P2score_tot << endl;
                cout << "TOTAL POINTS(%) ---> " << p1 << " " << (float(P1score_tot) / (float(gamecount) * 48)) * 100 << "  vs  ";
                cout << p2 << " " << (float(P2score_tot) / (float(gamecount) * 48)) * 100 << endl;
                if (p1w > p2w)
                    cout << "OVERALL WINNER: " << caps_p1;
                else if (p1w < p2w)
                    cout << "OVERALL WINNER: " << caps_p2;
                else
                    cout << "OVERALL DRAW";
                if (perfectturing)
                {
                    pt_comp.push_back(3 * p2w + (gamecount - (p1w + p2w)));
                    pt_count ++;
                    if (pt_comp.size() == pt_vectorsize)
                    {
                        cout << endl << endl << endl << "PERFECT TURING PROTOCOL OVER" << endl;
                        cout << "RESULTS :" << endl << endl;
                        float pt_worstcoef = 0;
                        for(int i = 0; i < pt_vectorsize; i++)
                        {
                            cout << "Coeffecient: " << fixed << setprecision(pt_dec) << i * pow(10, -pt_dec) << "  ---> Result: " << pt_comp[i] << endl;
                            if (pt_comp[i] >= pt_bestresult)
                            {
                                pt_bestresult = pt_comp[i];
                                pt_bestcoef = i * pow(10, -pt_dec);
                            }
                            if (pt_comp[i] <= pt_worstresult)
                            {
                                pt_worstresult = pt_comp[i];
                                pt_worstcoef = i * pow(10, -pt_dec);
                            }
                        }
                        cout << "The worst Coeffecient is " << pt_worstcoef << " with the result of " << pt_worstresult << " (out of a maximum of " << 3 * pt_strk << ")" << endl;
                        cout << "The best Coeffecient is " << pt_bestcoef << " with the result of " << pt_bestresult << " (out of a maximum of " << 3 * pt_strk << ")" << endl;
                        cout << "Turing Coeffecient set to " << pt_bestcoef << endl;
                        iacoef = pt_bestcoef;
                        perfectturing = false;
                    }
                }
                streak = 0;
                gamecount = 0;
                p1w = p2w = 0;
                P1score_tot = P2score_tot = 0;
                game_streak = false;
            }
            if (!game_streak && !perfectturing)
            {
                int rr = 1;
                cout << endl << endl << "Do you wanna Retry? (1 for Yes [or] 0 for No) : ";
                while (1)
                {
                    cin >> rr;
                    if (cin.fail() || rr < 0 || rr > 1)
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Input a valid answer (0 or 1)." << endl;
                        cout << "Do you wanna Retry? (1 for Yes [or] 0 for No) : ";
                    }
                    else
                        break;
                }
                if (rr == 0)
                    gameover = true;
                else
                    Retry();
            }
            else
                Retry();
        }
    }
    DrawLogo();
    if (gamecount_tot == 1)
        cout << endl << "During this play session, 1 game was played.";
    else
        cout << endl << "During this play session there were played " << gamecount_tot << " games.";
    cout << endl << "The End.";
}
