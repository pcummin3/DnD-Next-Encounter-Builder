#include <iostream>

using namespace std;

//  D&D GAME CONSTANTS
//  20 Levels in current iteration of 5e.  Change the limit and add exp values for new levels if "epic" play becomes available.
const int expNeededByLevel [20] = {0, 300, 900, 2700, 6500, 14000, 23000, 34000, 48000, 64000, 85000, 100000, 120000, 140000, 165000, 195000, 225000, 265000, 305000, 355000};

//  Proficiency would be limited to 20 levels due to PCs, but Monsters use the same proficiency rates plus and through lvl 30 (plus fractional lvls).  (lvl 0= 31, 1/8= 32, 1/4= 33, 1/2= 34)
const int proficiencyBonusByLevel [34] = {2, 2, 2, 2, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 2, 2, 2, 2};

//  5e Currently has attribute scores 1-30.
const int attributeModifierByLevel [30] = {-5, -4, -4, -3, -3, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10};

//  5e Currently has 30 monster levels plus 4 fractional levels (0, 1/8, 1/4, 1/2).  To keep levels 1 for 1 (like my other matrices), lvl 0= 31, 1/8= 32, 1/4= 33, 1/2= 34.
const int monsterXpByLevel [34] = {200, 450, 700, 1100, 1800, 2300, 2900, 3900, 5000, 5900, 7200, 8400, 10000, 11500, 13000, 15000, 18000, 20000, 22000, 25000, 33000, 41000, 50000, 62000, 75000, 90000, 105000, 120000, 135000, 155000, 10, 25, 50, 100};

//  Encounter Multipliers [p. 82 DMG].  Monster numbering 1-15+.
//  NOTE:  If PC party is fewer than 3, bump the multiplier up by one [max 5, aka matrix position 16]
//  NOTE:  If PC party is greater than 5, reduce the multiplier by one [min 0.5, aka matrix position 17]
const float encounterMultipliers [8] = {.5, 1, 1.5, 2, 2.5, 3, 4, 5};

//  XP Thresholds by Character Level [p. 82 DMG].  FOUR ARRAYS:  EASY, MEDIUM, HARD, DEADLY
//  EASY
const int easyXpThresholdByLevel [20] = {25, 50, 75, 125, 250, 300, 350, 450, 550, 600, 800, 1000, 1100, 1250, 1400, 1600, 2000, 2100, 2400, 2800};
//  MEDIUM
const int mediumXpThresholdByLevel [20] = {50, 100, 150, 250, 500, 600, 750, 900, 1100, 1200, 1600, 2000, 2200, 2500, 2800, 3200, 3900, 4200, 4900, 5700};
//  HARD
const int hardXpThresholdByLevel [20] = {75, 150, 225, 375, 750, 900, 1100, 1400, 1600, 1900, 2400, 3000, 3400, 3800, 4300, 4800, 5900, 6300, 7300, 8500};
//  DEADLY
const int deadlyXpThresholdByLevel [20] = {100, 200, 400, 500, 1100, 1400, 1700, 2100, 2400, 2800, 3600, 4500, 5100, 5700, 6400, 7200, 8800, 9500, 10900, 12700};

//  Adventuring Day XP [p. 84 DMG].  This is just a DM tool to have an idea of how much progress a DM should "expect" each PC to make in a day (add all the PCs together for party total).
const int adventuringDayXp [20] = {300, 600, 1200, 1700, 3500, 4000, 5000, 6000, 7500, 9000, 10500, 11500, 13500, 15000, 18000, 20000, 25000, 27000, 30000, 40000};

int playerLevels[6];  //  Declare the party's matrix, setting the length to a max of 6 players.
int enemyLevels[32];  //  Declare the enemy's matrix, setting the length to a maz of 32 enemies.


//  Function Prototypes
int mainMenu();
int fileSelect();
int runFileSelectMenu(int);
int partySize();
void enterPlayerLevels(int, int*);
int calculateThresholds(int*, const int*, int);
void displayThresholds(int&, int&, int&, int&);
int enemyElementSize();
void enterEnemyLevels(int, int*);
int calculateMonsterXp(int*, const int* ,int);
void displayEncounterChallenge(const float*, int, int, int, int, int, int, int);

int encounterBuilderMenu();
int runEncounterBuilderMenu(int);
void buildEasyEncounters(const float*, int, int, int, int, int, int, int);
void buildMediumEncounters(const float*, int, int, int, int, int, int, int);
void buildHardEncounters(const float*, int, int, int, int, int, int, int);
void buildDeadlyEncounters(const float*, int, int, int, int, int, int, int);



int main()
{
    int mainMenuSelection(0), mainMenuLoop(0), fileMenuSelection(0), pSize(0), eSize(0), countDown(0), monsterXp(0);
    int easyThreshold(0), mediumThreshold(0), hardThreshold(0), deadlyThreshold(0);
    int x(0), y(0), z(0);

    cout << endl << endl << "                      Welcome to Eric's C++ DM SCREEN!" << endl;
    cout << "                       _____________________________" << endl << endl << endl;

    //  fileSelect();


    while (mainMenuSelection !=6)
    {
        mainMenuSelection = mainMenu();

        switch(mainMenuSelection)
        {

            case 1:  // Main Menu Choice:  1.  Enter Party Values
            {
                pSize = partySize();  //  Return party size
                if (pSize > 6)
                {
                    pSize = 6;
                }

                enterPlayerLevels(pSize, playerLevels);  //  Calls function for user to populate the party matrix with the PC levels

                //  Determine Party Thresholds
                easyThreshold = calculateThresholds(playerLevels, easyXpThresholdByLevel, pSize);  //  Easy
                mediumThreshold =  calculateThresholds(playerLevels, mediumXpThresholdByLevel, pSize);  //  Medium
                hardThreshold = calculateThresholds(playerLevels, hardXpThresholdByLevel, pSize);  //  Hard
                deadlyThreshold = calculateThresholds(playerLevels, deadlyXpThresholdByLevel, pSize);  //  Deadly


                break;
            }

            case 2:
            {
                displayThresholds(easyThreshold, mediumThreshold, hardThreshold, deadlyThreshold);

                //  PC Daily limit

                while (x < pSize)
                {
                    y = playerLevels[x];
                    z = adventuringDayXp[(y - 1)] + z;
                    x++;
                }

                cout << endl << "The player's can be" << endl << "expected to earn roughly  " << z;
                cout << "\t   XP in one day!" << endl << endl;

                break;
            }

            case 3:
            {
                eSize = enemyElementSize();  //  Return enemy element size

                enterEnemyLevels(eSize, enemyLevels);  //  Calls function for user to populate the enemy's matrix with monster levels

                //Calculate Enemy Group XP Value
                monsterXp = calculateMonsterXp(enemyLevels, monsterXpByLevel, eSize);

                cout << endl << endl << "Unmodified Monster Xp Value:  " << monsterXp << endl << endl;

                break;

            }

            case 4:
            {
                //  Determine encounter challenge
                displayEncounterChallenge (encounterMultipliers, pSize, eSize, monsterXp, easyThreshold, mediumThreshold, hardThreshold, deadlyThreshold);
                break;
            }

            case 5:
            {
                mainMenuSelection = fileSelect();

                break;
            }

            case 6:
            {

                break;
            }

            case 7:
            {
                mainMenuSelection = encounterBuilderMenu();
                break;
            }
        }
    }

    return 0;
}

int mainMenu()
{
    int mainMenuSelection(0);

    while(true)
    {
        cout << endl;
        cout << "        *********************        " << endl;
        cout << "       |     -MAIN MENU-     |       " << endl;
        cout << "   *******************************   " << endl;
        cout << "*************************************" << endl;
        cout << "*  1.  Enter Party Values           *" << endl;
        cout << "*  2.  Display Party Thresholds     *" << endl;
        cout << "*  3.  Build Encounter              *" << endl;
        cout << "*  4.  Determine Enc Challenge      *" << endl;
        cout << "*  5.  File Select Menu             *" << endl;
        cout << "*  6.  EXIT                         *" << endl;
        cout << "*  7.  Encounter Builder Menu       *" << endl;
        cout << "*************************************" << endl;
        cin >> mainMenuSelection;

        if (mainMenuSelection < 8 && mainMenuSelection > 0)
        {
            return mainMenuSelection;
        }
        cout << "Invalid Selection-" << endl << endl;
    }

    //return mainMenuSelection;
}

int partySize()
{
    int partySize(0);

    cout << endl;
    cout << "Please enter the number PCs in your party:" << endl;
    cin >> partySize;

    return partySize;
}

void enterPlayerLevels(int pSize, int* playerLevels)
{
    int countDown(0), levelValue(0);
    countDown = 0;

    while (countDown < pSize)
    {
        cout << endl;
        cout << "Please enter Player " << countDown+1 << "'s level:" << endl;
        cin >> levelValue;  //  User inputs the level for the next Player#

        if (levelValue < 1 || levelValue > 20)
        {
            cout << endl << "You must enter a value between levels 1 and 20.";
            cout << endl << "Please re-enter the player's level." << endl;
            countDown--;
        }

        playerLevels[countDown] = levelValue;
        countDown++;  //  Counter, change the matrix position for input, and determine Player #
    }
}

int calculateThresholds(int* playerLevels, const int* thresholdValues, int pSize)
{
    int countDown(0), thresholder(0), currentPlayerLevel(0);

    while (countDown < pSize)
    {
        currentPlayerLevel = playerLevels[countDown] -1;
        thresholder = thresholder + (thresholdValues[currentPlayerLevel]);
        countDown++;
    }

    return thresholder;
}

void displayThresholds(int& easy, int& medium, int& hard, int& deadly)
{
    cout << endl << endl << "  -Party Thresholds-" << endl << endl;
    cout << "Easy Party Threshold:     " << easy << "\t   XP" << endl;
    cout << "Medium Party Threshold:   " << medium << "\t   XP" << endl;
    cout << "Hard Party Threshold:     " << hard << "\t   XP" << endl;
    cout << "Deadly Party Threshold:   " << deadly << "\t   XP" << endl;
}

int enemyElementSize()
{
    int enemyElementSize(0);

    cout << endl;
    cout << "Please enter the number Monsters pitted against your players:" << endl;

    while (enemyElementSize == 0)
    {
        cin >> enemyElementSize;

        if (enemyElementSize < 1 || enemyElementSize > 32)
        {
            cout << endl << endl << "You have entered an invalid number of enemies." << endl;
            cout << "Please select a number between 1 and 32." << endl << endl;
            enemyElementSize = 0;
        }
    }

    return enemyElementSize;
}

void enterEnemyLevels(int eSize, int* enemyLevels)
{
    int countDown(0);
    float levelValue(0);
    countDown = 0;

    while (countDown < eSize)
    {
        cout << endl;
        cout << "Please enter Enemy #  " << countDown+1 << "'s level:" << endl;
        cin >> levelValue;  //  User inputs the level of a Monster.  Must store to a float and convert into an integer to account for (1/8, 1/4, 1/2 levels)

        if (levelValue == 0)
        {
            levelValue = 31;
        }

        if (levelValue == .125)
        {
            levelValue = 32;
        }

        if (levelValue == .25)
        {
            levelValue = 33;
        }

        if (levelValue == .5)
        {
            levelValue = 34;
        }

        if (levelValue < 1 || levelValue >34)
        {
            cout << endl << "You must enter a value between levels 0 and 30.";
            cout << endl << "[Or 1/8, 1/4, 1/2] [Or .125, .25, .5]";
            cout << endl << "Please re-enter the Monster's level." << endl;
            countDown--;
        }

        enemyLevels[countDown] = levelValue;  //  Value has already been validated as an integer
        countDown++;  //  Counter, change the matrix position for input, and determine Enemy #
    }
}

int calculateMonsterXp(int* enemyLevels, const int* monsterXpByLevel, int eSize)
{
    int countDown(0), monsterXp(0), enemyLevel(0);

    while (countDown < eSize)
    {
        enemyLevel = enemyLevels[countDown];
        monsterXp = monsterXpByLevel[(enemyLevel - 1)] + monsterXp;
        countDown++;
    }

    return monsterXp;
}

void displayEncounterChallenge(const float* encounterMultipliers , int pSize, int eSize, int monsterXp, int easy, int medium, int hard, int deadly)
{
    float multiplier(0);
    string difficulty;

    if (eSize > 2 && eSize < 7)
    {
        eSize = 3;
    }

    if (eSize > 6 && eSize < 11)
    {
        eSize = 4;
    }

    if (eSize > 10 && eSize < 15)
    {
        eSize = 5;
    }

    if (eSize > 15)
    {
        eSize = 6;
    }

    if (pSize < 3)
    {
        eSize++;
    }

    if (pSize > 5)
    {
        eSize--;
    }

    multiplier = encounterMultipliers[eSize];
    monsterXp = monsterXp * multiplier;

    if (monsterXp < easy)
    {
        difficulty = "Insignificant";
    }

    else if (monsterXp < medium)
    {
        difficulty = "Easy";
    }

    else if (monsterXp < hard)
    {
        difficulty = "Medium";
    }

    else if (monsterXp < deadly)
    {
        difficulty = "Hard";
    }

    else
    {
        difficulty = "Deadly";
    }

    cout << endl << endl << "  -Enemy Encounter-" << endl << endl;
    cout << "Modified Encounter XP Value:\t" << monsterXp << "\t   XP" << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << "Encounter Difficulty:t\t" << difficulty << endl;
    cout << "------------------------------------------------------------------" << endl;
}

int encounterBuilderMenu()
{
        int encounterBuilderMenuSelection(0);

    while(true)
    {
        cout << endl;
        cout << "        *********************        " << endl;
        cout << "       | -Encounter Builder- |       " << endl;
        cout << "   *******************************   " << endl;
        cout << "*************************************" << endl;
        cout << "*  1.  Return to Main Menu          *" << endl;
        cout << "*  2.  Find Easy Encounters         *" << endl;
        cout << "*  3.  Find Medium Encounters       *" << endl;
        cout << "*  4.  Find Hard Encounters         *" << endl;
        cout << "*  5.  Find Deadly Encounters       *" << endl;
        cout << "*  6.  EXIT                         *" << endl;
        cout << "*************************************" << endl;
        cin >> encounterBuilderMenuSelection;

        if (encounterBuilderMenuSelection < 7 && encounterBuilderMenuSelection > 0)
        {
            encounterBuilderMenuSelection = runEncounterBuilderMenu(encounterBuilderMenuSelection);

            if (encounterBuilderMenuSelection < 0 || encounterBuilderMenuSelection > 6)
            {
                cout << "Invalid Selection-" << endl << endl;
            }

            if (encounterBuilderMenuSelection == 1 || encounterBuilderMenuSelection == 6)
            {
                return encounterBuilderMenuSelection;
            }
        }
    }
}

int runEncounterBuilderMenu(int Selection)
{
        int encounterBuilderSelectLoop(0);

    while (encounterBuilderSelectLoop == 0)
    {

        switch(Selection)
        {

            case 1:  // File Select Choice:  1.  Continue to Main Menu
            {
                return Selection;
                break;
            }

            case 2:
            {
                cout << endl << "Build Easy Encounter" << endl;
                return Selection;
                break;
            }

            case 3:
            {
                cout << endl << "Build Medium Encounter" << endl;
                return Selection;
                break;
            }

            case 4:
            {
                cout << endl << "Build Hard Encounter" << endl;
                return Selection;
                break;
            }

            case 5:
            {
                cout << endl << "Build Deadly Encounter" << endl;
                return Selection;
                break;
            }

            case 6:
            {
                return Selection;
                break;
            }
        }
    }
}

void buildEasyEncounters(const float* encounterMultipliers , int pSize, int eSize, int monsterXp, int easy, int medium, int hard, int deadly)
{



}

int fileSelect()
{
    int fileMenuSelection(0);

    while(true)
    {
        cout << endl;
        cout << "        *********************        " << endl;
        cout << "       |     -File Select-   |       " << endl;
        cout << "   *******************************   " << endl;
        cout << "*************************************" << endl;
        cout << "*  1.  Return to Main Menu          *" << endl;
        cout << "*  2.  Load Previous Party          *" << endl;
        cout << "*  3.  Load Previous Encounter      *" << endl;
        cout << "*  4.  Save New Party               *" << endl;
        cout << "*  5.  Save New Encounter           *" << endl;
        cout << "*  6.  EXIT                         *" << endl;
        cout << "*************************************" << endl;
        cin >> fileMenuSelection;

        if (fileMenuSelection < 7 && fileMenuSelection > 0)
        {
            fileMenuSelection = runFileSelectMenu(fileMenuSelection);

            if (fileMenuSelection < 0 || fileMenuSelection > 6)
            {
                cout << "Invalid Selection-" << endl << endl;
            }

            if (fileMenuSelection == 1 || fileMenuSelection == 6)
            {
                return fileMenuSelection;
            }
        }
    }
}

int runFileSelectMenu(int fileMenuSelection)
{
    int fileSelectLoop(0);

    while (fileSelectLoop == 0)
    {

        switch(fileMenuSelection)
        {

            case 1:  // File Select Choice:  1.  Continue to Main Menu
            {
                return fileMenuSelection;
                break;
            }

            case 2:
            {
                cout << endl << "Load Previous Party" << endl;
                return fileMenuSelection;
                break;
            }

            case 3:
            {
                cout << endl << "Load Previous Encounter" << endl;
                return fileMenuSelection;
                break;
            }

            case 4:
            {
                cout << endl << "Save New Party" << endl;
                return fileMenuSelection;
                break;
            }

            case 5:
            {
                cout << endl << "Save New Encounter " << endl;
                return fileMenuSelection;
                break;
            }

            case 6:
            {
                return fileMenuSelection;
                break;
            }
        }
    }
}
