#include <iostream>
#include <memory>

using namespace std;

class Character {
	bool blockOnOff{ false };

public:
	virtual int Attack() = 0;
	virtual void TakeDamage(int atkDamage) = 0;
	virtual void Block() {
		cout << "Blocking Next Attack. Defense doubled for next turn.\n";
		SetBlockOn();
	}
	virtual void UnBlock() {
		SetBlockOff();
	}

protected:
	bool GetBlockBool() {
		return blockOnOff;
	}
	void SetBlockOn() {
		blockOnOff = true;
	}
	void SetBlockOff() {
		blockOnOff = false;
	}
};

class Player : public Character {
	int health;
	int maxHealth;
	int meleeDamage;
	int rangeDamage;
	const int swordDamage{ 20 };
	const int bowDamage{ 20 };
	int defense;
	int shieldDefense{ 0 };
	int specialAbility;
	string items[5];
	int compareValue{ 1 };
	int compareValue2{ 0 };
	bool rangedAttacker{ false };
public:
	Player() : health{ 75 }, maxHealth{ health }, meleeDamage{ 10 }, rangeDamage{ 15 }, defense{ 0 }, specialAbility{ 0 } {}
	//loads final boss level player constructor/initializer
	Player(int health, int meleeDamage, int rangeDamage, int defense, int specialAbility) : health{ health }, maxHealth{ health }, meleeDamage{ meleeDamage }, rangeDamage{ rangeDamage }, defense{ defense }, specialAbility{ specialAbility }, items{ {"map"}, {"sword"}, {"shield"}, {"armour"}, {"bow"} } {}

	int Attack() override {
		int damage{};
		int playerChoice = TakePlayerChoice();
		DamageModifier(playerChoice, damage);
		SpecialAbilityCriticalHits(damage);
		SpecialAbilityLifeSteal();
		cout << "==========================\n";
		cout << "Performing Attack.\n";
		cout << "Damage Points: " << damage << "\n";
		cout << "==========================\n";
		return damage;
	}

	void TakeDamage(int atkDamage) override {
		if (!rangedAttacker) {
			atkDamage = DefenseModifier(atkDamage);
			SpecialAbilityBlocker(atkDamage);
			SetHealth(atkDamage);
		}
		else {
			cout << "avoided taking any damage.\n";
		}
	}
	void TakeDamage(int atkDamage, bool groundSlash, bool speedDash) {
		if (!rangedAttacker) {
			if (!groundSlash && !speedDash) {
				atkDamage = DefenseModifier(atkDamage);
				SpecialAbilityBlocker(atkDamage);
				SetHealth(atkDamage);
			}
			if (groundSlash && !speedDash) {
				atkDamage = DefenseModifier(atkDamage, groundSlash);
				SpecialAbilityBlocker(atkDamage);
				SetHealth(atkDamage);
			}
			if (speedDash && !groundSlash) {
				SetHealth(atkDamage);
			}
		}
		else {
			cout << "avoided taking any damage.\n";
		}
	}
	void Block() override {
		if (ItemCheck("shield"))
			shieldDefense = 30;
		SetBlockOn();
	}
	void UnBlock() override {
		shieldDefense = 0;
		SetBlockOff();
	}
	void AddItems(const int& levelOn, const string& itemName) {
		const int& tempValue = levelOn - 1;
		if (IsNumValid(tempValue)) {
			items[tempValue] = itemName;
		}
		else {
			cout << "ERROR: trying to access out of bound array index.\n";
		}
	}
	//getters
	const string GetItemSingle(const int& levelOn) {
		const int& tempValue = levelOn - 1;
		if (IsNumValid(tempValue)) {
			return items[tempValue];
		}
		else {
			cout << "ERROR: trying to access out of bound array index.\n";
			return 0;
		}
	}
	const int GetHealth() {
		return health;
	}
	//getters print function
	void ShowPlayerStats() {
		cout << "====================  Player Stats  ====================\n";
		cout << "Health: " << health << "\n";
		cout << "Base Melee Damage: " << meleeDamage << "\n";
		cout << "Base Range Damage: " << rangeDamage << "\n";
		cout << "Base Defense Value: " << defense << "\n\n";
	}
	void ShowSpecialAbility() {
		cout << "===================================================================================================\n";
		switch (specialAbility) {
		case 0: break;
		case 1:
		{
			cout << "Special Ability: Critical Hits.\nDamage Points multiplied by 3.\n";
			ProbablityAnnouncer();
			cout << "-------------------------------------------------------------------\n";
			break;
		}
		case 2:
		{
			cout << "Special Ability: Critical Hits.\nDamage Points multiplied by 3.\n";
			cout << "Special Ability: Blocker.\nImmune to damage for the next turn.\n";
			ProbablityAnnouncer();
			cout << "-------------------------------------------------------------------\n";
			break;
		}
		case 3:
		{
			cout << "Special Ability: Critical Hits.\nDamage Points multiplied by 3.\n";
			cout << "Special Ability: Blocker.\nImmune to damage for the next turn.\n";
			cout << "Special Ability: Life Steal.\nHeal by a random amount 1 to 20.\n";
			ProbablityAnnouncer();
			cout << "-------------------------------------------------------------------\n";
			break;
		}
		case 4:
		{
			cout << "Special Ability: Critical Hits.\nDamage Points multiplied by 3.\n";
			cout << "Special Ability: Blocker.\nImmune to damage for the next turn.\n";
			cout << "Special Ability: Life Steal.\nHeal by a random amount 1 to 20.\n";
			cout << "Special Ability: Range Attacker.\nImmune to damage for the next turn. Note: only applicable while performing range attacks.\n";
			ProbablityAnnouncer();
			cout << "------------------------------------------------------------------------------------------------\n";
			break;
		}
		}
		cout << "===================================================================================================\n";
	}
	void ProbablityAnnouncer() {
		cout << "Probability of Special Ability Activating is 10% for normal level. 20% for final level.\n";
	}
	//setter-public
	void SetCompareValue() {
		compareValue = RNG1to10();
	}
	void SetSecondCompareValue() {
		compareValue2 = RNG1to10();
	}
	void SetHealthToMax() {
		health = maxHealth;
	}
	void LevelUp(const int& levelOn, const int& totalLevel) {
		//increase hp, damage, defense by 5 points
		this->IncreaseStatsByFive();
		//player reset health values
		this->SetHealthToMax();
		//show player stats
		this->ShowPlayerStats();
		//increase specialAbility by 1
		if (levelOn > 1 && levelOn < totalLevel)
			this->AddSpecialAbility();
		//show specialAbility
		ShowSpecialAbility();
	}

protected:
	bool IsNumValid(const int& tempValue) {
		return (tempValue >= 0 && tempValue < 5);
	}

	bool isAlive() {
		return health > 0;
	}
	//setter protected
	void SetHealth(int atkDamage) {
		if (atkDamage < 0)
			atkDamage = 0;
		if (isAlive()) {
			cout << "Enemy Attack damages player by: " << atkDamage << "\n";
			health = health - atkDamage;
			health > 0 ? health : health = 0;
			cout << "Player Health Remaining: " << health << "\n";
		}
		else {
			cout << "Player is dead.\n";
		}
	}
	//Level increase Modified setter
	void IncreaseStatsByFive() {
		maxHealth += 5;
		meleeDamage += 5;
		rangeDamage += 5;
		defense += 5;
	}
	void AddSpecialAbility() {
		specialAbility++;
	}
	int RNG1to10() {
		return (rand() % 10) + 1;
	}
	int TakePlayerChoice() {
		int playerChoice{};
		cout << "Choose Damage Type: \n";
		cout << "Press 1 for Melee.\nPress 2 for Ranged.\n";
		cin >> playerChoice;
		if (playerChoice == 2 && !ItemCheck("bow")) {
			cout << "Bow needed to perform Range Attacks.\n";
			cout << "Performing Melee Attack instead.\n";
			playerChoice = 1;
		}
		return playerChoice;
	}
	void DamageModifier(int playerChoice, int& damage) {
		rangedAttacker = false;
		switch (playerChoice) {
		case 1:
			damage = meleeDamage + (ItemCheck("sword") ? swordDamage : 0);
			break;
		case 2:
		{
			SpecialAbilityRangedAttacks();
			damage = rangeDamage + (ItemCheck("bow") ? bowDamage : 0);
			break;
		}
		default:
			cout << "Wrong option.";
			break;
		}
	}
	int DefenseModifier(int atkDamage) {
		int tempDefenseValue = defense + shieldDefense + (ItemCheck("armour") ? defense : 0);
		if (GetBlockBool()) {
			atkDamage -= int(tempDefenseValue / 2);
		}
		else {
			atkDamage -= int(tempDefenseValue / 3);
		}
		return atkDamage;
	}
	int DefenseModifier(int atkDamage, bool groundSlash) {
		if (groundSlash) {
			return atkDamage - defense;
		}
		else {
			return DefenseModifier(atkDamage);
		}
	}
	bool ItemCheck(const string& itemName) {
		for (const string& item : items) {
			if (item == itemName) {
				return true;
			}
		}
		return false;
	}
	bool ProbabilityChecker() {
		int randomNumber = RNG1to10();
		if (randomNumber == compareValue || randomNumber == compareValue2)
			return true;
		return false;
	}
	void SpecialAbilityCriticalHits(int& damage) {
		if (specialAbility >= 1 && ProbabilityChecker()) {
			cout << "Special Ability(Passive) Critical Hits: Activated\n";
			cout << "Critical Hit! 3x Damage!\n";
			damage *= 3;
		}
	}
	void SpecialAbilityBlocker(int& atkDamage) {
		if (specialAbility >= 2 && ProbabilityChecker()) {
			cout << "Special Ability(Passive) Blocker: Activated\n";
			cout << "You blocked the attack completely. No damage.\n";
			atkDamage = 0;
		}
	}
	void SpecialAbilityLifeSteal() {
		if (specialAbility >= 3 && ProbabilityChecker()) {
			cout << "Special Ability(Passive) Life-Steal: Activated\n";
			if (GetHealth() < maxHealth) {
				int healAmount = (rand() % 20) + 1;
				health += healAmount;
				cout << "Healed By: " << healAmount << "\n";
				cout << "Health Value: " << health << "\n";
			}
			else {
				cout << "Health is full.\n";
			}
		}
	}
	void SpecialAbilityRangedAttacks() {
		if (specialAbility >= 4 && ProbabilityChecker()) {
			cout << "Special Ability(Passive) Ranged-Attacker: Activated\n";
			cout << "Immune to damage for the next turn.\n";
			rangedAttacker = true;
		}
	}
};

class EnemyMurloc : public Character {
	int health;
	int maxHealth;
	int damage;
	int defense;

public:
	EnemyMurloc(int value) : health{ 30 + 5 * value }, damage{ 10 + 5 * value }, defense{ 0 + 5 * value } {}
	EnemyMurloc(int health, int damage, int defense) : health{ health }, maxHealth{ health }, damage{ damage }, defense{ defense } {}
	int Attack() {
		int tempDamage = damage;
		cout << "Enemy Attack Damage: " << tempDamage << "\n";
		return tempDamage;
	}
	void TakeDamage(int damage) {
		if (isAlive()) {
			int tempDef = defense + (GetBlockBool() ? defense : 0);
			if (tempDef > damage)
				damage = 0;
			damage -= tempDef;
			health -= damage;
			health > 0 ? health : health = 0;
			cout << "Damaged Inflicted on Enemy Murloc: " << damage << "\n";
			cout << "Enemy Murloc health remaining: " << health << "\n";
		}
		else {
			cout << "Enemy Murloc is already dead.\n";
		}
		if (health < 0)
			health = 0;
	}
	const int& GetHealth() {
		return health;
	}
	const int& GetDamage() {
		return damage;
	}
	void IncreaseStats() {
		maxHealth += 5;
		damage += 5;
		defense += 5;
	}
	void SetHealthToMax() {
		health = maxHealth;
	}

protected:
	bool isAlive() {
		return health > 0;
	}
};

class BossEnemyMurloc : public EnemyMurloc {
	bool groundSlash{ false };
	bool speedDash{ false };
	int luckyNumberRandom;

public:
	BossEnemyMurloc(int health, int damage, int defense) : EnemyMurloc(health, damage, defense) {}

	int Attack() override {
		return AttackType();
	}
	bool IsGroundSlash() {
		return groundSlash;
	}
	bool IsSpeedDash() {
		return speedDash;
	}
	//passive health regen probability 20%
	void PassiveHealthRegen() {
		if (rand() % 5 == luckyNumberRandom) {
			cout << "Enemy Special Ability: Activated\n Ability Name: Full Health Regen.\n";
			SetHealthToMax();
			cout << "Health: " << GetHealth() << "\n";
		}
	}
	void LoadLuckyNumber() {
		luckyNumberRandom = rand() % 5;
	}

protected:
	int AttackType() {
		speedDash = false;
		groundSlash = false;
		int atkChoice = rand() % 3;
		switch (atkChoice) {
		case 0:
		{
			cout << "Normal Attack performed.\n";
			return EnemyMurloc::Attack();
			break;
		}
		case 1:
		{
			cout << "Performing Ground Slash.\n";
			return GroundSlash();
			break;
		}
		case 2:
		{
			cout << "Performing Speed Dash.\n";
			return SpeedDash();
			break;
		}
		}
	}
	int GroundSlash() {
		groundSlash = true;
		int damage = GetDamage();
		return damage + (damage / 3);
	}
	int SpeedDash() {
		speedDash = true;
		int damage = GetDamage();
		return damage - (damage / 3);
	}
};

class Game {
private:
	unique_ptr<Player> playerHero{};
	unique_ptr<EnemyMurloc> enemy{};
	unique_ptr<BossEnemyMurloc> bossEnemy{};
	bool gameOnOff{ true };
	int levelOn{};
	int totalLevel{ 6 };
	int enemyCount;
	string lootItems[5];

public:

	Game() : gameOnOff{ true }, levelOn{ 1 }, totalLevel{ 6 }, lootItems{ {"map"}, {"sword"}, {"shield"}, {"armour"}, {"bow"} } {}

	void StartGameLoop() {  //Main Outer Loop
		WelcomeMessage();
		InitializePlayer();
		while (gameOnOff) {
			LevelLoop();
			if (levelOn == totalLevel) {
				BossLevelLoop();
			}
		}
	}

protected:
	void WelcomeMessage() {
		cout << "*****************************************************************************************************\n";
		cout << "Once upon a long-long time ago, there was a village situated in the farthest corners of the world.\n";
		cout << "It was attacked frequently by murlocs.\nIn one such raid the murlocs had managed to capture some humans alive.\n";
		cout << "The murlocs were a very old race of goblins and lived in an underground lair of complex tunnels and levels.\n";
		cout << "Generally they would take children captive who would be cooked and fed to the boss murloc.\nHowever today they have taken our hero who if he can wake up on time will be presented with the opportunity to save his own life and the lives of his fellow villagers if he can destroy the murlocs once and for all.\n";
		cout << "Alright so before we begin, let me explain the rules quickly.\nYou can perform only attack and block as options to choose when your fighting with enemies. Depending on the type of weapon u have the attack will be performed accordingly.\nIf you have sword you can do melee damage. If you have shield you can block attacks with more defense. If you have bow you can shoot infinite arrows.\nThere is 6 levels to this game and in the final level you will face the boss murloc.\nOnce you have defeated the final boss you win the game and can return to your home with pride, honor and glory.\n";
		cout << "*****************************************************************************************************\n";
	}
	void InitializePlayer() {
		playerHero = unique_ptr<Player>(new Player());

		cout << "\nAs our hero comes to his senses he realises all his weapons shield and armour have been stolen from him.\n";
		cout << "Looking around him, he sees one murloc around him who seems to be drawing a map.\n";
		cout << "Quick as a fox our player jumps up behind the murloc ready to make the first move.\n";
	}

	unique_ptr<EnemyMurloc> EnemyInitializer() {
		const int& tempValue = levelOn - 1;
		if (IsNumValid(tempValue)) {
			return unique_ptr<EnemyMurloc>(new EnemyMurloc(tempValue));
		}
		else {
			cout << "ERROR: Tried accessing out of bound array index.\n";
			return 0;
		}
	}

	unique_ptr<BossEnemyMurloc> BossEnemyInitializer() {
		return unique_ptr<BossEnemyMurloc>(new BossEnemyMurloc(100, 50, 30));
	}

	void LevelLoop() {
		enemy = EnemyInitializer();
		enemyCount = levelOn;
		cout << "\nCurrent Level: " << levelOn << "\n";
		cout << "Total Enemies to defeat: " << enemyCount << "\n";
		//main level loop
		while (playerHero->GetHealth() > 0 && enemyCount > 0) {
			int playerChoice = TakePlayerChoice();
			PlayPlayerChoice(playerChoice, enemy);
			EnemyMove(enemy);
			CheckGameStatus(enemy);
		}
		//After level loop exits check condition for win or loss.
		playerHero->GetHealth() > 0 && enemyCount == 0 ? LevelWon() : LevelLost();
	}

	void BossLevelLoop() {
		//initialize boss enemy
		bossEnemy = BossEnemyInitializer();
		enemyCount = 1;
		cout << "=======================================================================\n";
		cout << "========================  BOSS   LEVEL  ===============================\n";
		cout << "=======================================================================\n";
		//cout << "Enters Boss Murloc\n";

		//set comparevalue2
		if (levelOn == totalLevel)
			playerHero->SetSecondCompareValue();

		//initialize level loop- turn based moves
		while (playerHero->GetHealth() > 0 && enemyCount > 0) {
			int playerChoice = TakePlayerChoice();
			PlayPlayerChoice(playerChoice, bossEnemy);
			bossEnemy->LoadLuckyNumber();
			EnemyMove(bossEnemy);
			CheckGameStatus(bossEnemy);
		}
		(playerHero->GetHealth() > 0 && enemyCount == 0) ? GameWon() : LevelLost();
	}

	template <typename T>
	void EnemyMove(T& ptr) {
		//reset block move
		ptr->UnBlock();
		int enemyChoice = rand() % 2;
		switch (enemyChoice) {
		case 0:
		{
			int damage = ptr->Attack();
			playerHero->TakeDamage(damage);
			break;
		}
		case 1:
		{
			ptr->Block();
			break;
		}
		}
	}
	void EnemyMove(unique_ptr<BossEnemyMurloc>& ptr) {
		//perform random attacks
		ptr->PassiveHealthRegen();
		int damage = ptr->Attack();
		playerHero->TakeDamage(damage, ptr->IsGroundSlash(), ptr->IsSpeedDash());
	}

	int TakePlayerChoice() {
		int playerChoice;
		cout << "--------------------------------------------\n";
		cout << "Type your choice and press enter:\n";
		cout << "1. Press 1 to perform attack.\n";
		cout << "2. Press 2 to perform block\n";
		cout << "--------------------------------------------\n";
		cin >> playerChoice;
		return playerChoice;
	}

	template <typename T>
	void PlayPlayerChoice(int playerChoice, T& ptr) {
		//reset block move
		playerHero->UnBlock();
		switch (playerChoice) {
		case 1:
		{
			int damage = playerHero->Attack();
			ptr->TakeDamage(damage);
			break;
		}
		case 2:
		{
			playerHero->Block();
			break;
		}
		}
	}

	template <typename T>
	void CheckGameStatus(T& ptr) {
		//display player and enemy health
		cout << "Player Health Remaining: " << playerHero->GetHealth() << "\n";
		cout << "Enemy Health Remaining: " << ptr->GetHealth() << "\n";

		//check player is alive
		playerHero->GetHealth() > 0 ? gameOnOff = true : gameOnOff = false;

		//check enemy is alive
		if (ptr->GetHealth() <= 0) {
			//reset compare value after enemy dies
			playerHero->SetCompareValue();
			cout << "Enemy killed.\n";
			enemyCount--;
			if (enemyCount > 0) {
				ResetEnemyPtr();
				cout << "Enemies remaining: " << enemyCount << "\n";
				cout << "Enemy Health: " << ptr->GetHealth() << "\n";
			}
		}
	}

	void ResetEnemyPtr() {
		// to reset you can also set health to max and decrement enemy count
		const int& tempValue = levelOn - 1;
		if (IsNumValid(tempValue)) {
			enemy.reset(new EnemyMurloc(tempValue));
		}
		else {
			cout << "ERROR: trying to access out of bound array index.\n";
		}
	}

	void GameWon() {
		gameOnOff = false;
		cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
		cout << "Congratulations. You have won. Now you are free of the murlocs forever.\n";
		cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
	}

	void LevelWon() {
		cout << "====================================\n";
		cout << "Total Murlocs defeated: " << levelOn << "\n";
		cout << "Level completed.\n";
		cout << "====================================\n";

		//receive item into player member variable array
		const int& tempValue = levelOn - 1;
		if (IsNumValid(tempValue)) {
			const string& itemName = lootItems[tempValue];
			cout << "Received item: " << itemName << "\n";
			playerHero->AddItems(levelOn, itemName);
		}

		playerHero->LevelUp(levelOn, totalLevel);

		//increase levelOn by 1
		if (levelOn < totalLevel)
			levelOn++;
	}

	bool IsNumValid(const int& tempValue) {
		return (tempValue >= 0 && tempValue < 5);
	}

	void LevelLost() {
		cout << "You have died. Better luck next time.\n";
		int playerChoice{};
		cout << "Press 0 to exit\n";
		cout << "Press 1 to Restart level\n";
		cin >> playerChoice;
		switch (playerChoice) {
		case 0:
			gameOnOff = false;
			break;
		case 1:
		{
			gameOnOff = true;
			playerHero->SetHealthToMax();
			enemy->SetHealthToMax();
			break;
		}
		default:
		{
			cout << "Bad input. Exiting\n";
			gameOnOff = false;
			break;
		}
		}
	}
};

int main() {
	srand(time(0));
	unique_ptr<Game> game = unique_ptr<Game>(new Game());
	game->StartGameLoop();
}