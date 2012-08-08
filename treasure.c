
#include <stdio.h>
#include <stdlib.h>

#include "dice.h"

#include "treasure.h"

/* Makes a roll on the Coins Table */
static void do_coins(int luck);
/* Makes a roll on the Gems Table */
static void do_gems(int luck);
/* Generate a single random gem. */
static void generate_gem(int luck);

static char *generate_ornamental(int *base_value);
static char *generate_semiprecious(int *base_value);
static char *generate_fancy(int *base_value);
static char *generate_gemstone(int *base_value);
static char *generate_jewel(int *base_value);

static char *get_jeweled_item(int luck);
static char *get_jeweled_weapon(int luck, int magical);

/* Will make all the rolls necessary, and display the treasure
 * that was found to the user as best as it can. */
void display_treasure(int luck)
{
	int main_roll = roll_2d6daro() + roll_level(luck);

	/* printf("Rolled %d on Main Treasure Table\n", main_roll); */

	if(main_roll <= 4)
	{
		printf("All they found was trash.  Better luck next time!\n");
	}
	else if(main_roll <= 9)
	{
		/* printf("They found coins\n"); */
		do_coins(luck);
	}
	else if(main_roll <= 12)
	{
		/* printf("They found gems\n"); */
		do_gems(luck);
	}
	else
	{
		/* printf("Gems and coins were found!\n"); */
		do_coins(luck);
		do_gems(luck);
	}
}

/* Makes a roll on the Coins Table */
static void do_coins(int luck)
{
	int times = roll_xdy(1, 3);
	int i;
	int type_num; 
	char *type;

	for(i = 0; i < times; i++)
	{
		type_num = roll_2d6daro() + roll_level(luck);
		if(type_num <= 6)
			type = "Copper Coins";
		else if(type_num <= 15)
			type = "Silver Coins";
		else if(type_num <= 22)
			type = "Gold Coins";
		else if(type_num <= 30)
			type = "Platinum Coins";
		else if(type_num <= 40)
			type = "Jade Coins";
		else
			type = "Mithril Coins";

		printf("Found %d %s\n", roll_xdy(3, 6) * 10, type); 
	}
}

/* Makes a roll on the Gems Table */
static void do_gems(int luck)
{
	int count;
	int count_roll = roll_2d6daro() + roll_level(luck);
	int i;

	if(count_roll <= 7)
		count = 1;
	else if(count_roll <= 12)
		count = 2;
	else if(count_roll <= 16)
		count = 3;
	else if(count_roll <= 20)
		count = 4;
	else if(count <= 25)
		count = 5;
	else
		count = 10;

	printf("Found %d gems\n", count);

	for(i = 0; i < count; i++)
		generate_gem(luck);
}

/* Generate a single random gem. */
static void generate_gem(int luck)
{
	int size_roll = roll_2d6daro() + roll_level(luck);
	int quality_roll = roll_2d6daro() + roll_level(luck);
	int type_roll = roll_2d6daro() + roll_level(luck);
	int magic_roll = roll_2d6daro() + roll_level(luck);
	int setting_roll = roll_2d6daro() + roll_level(luck);

	double multiplier = 1;
	char *size;

	char *quality;
	char *magic = "no magical properties";

	char *type;
	int base_value;

	char *setting;

	if(size_roll <= 10)
	{
		size = "tiny";
		multiplier *= .5;
	}
	else if(size_roll <= 19)
	{
		size = "small";
		multiplier *= 1;
	}
	else if(size_roll <= 25)
	{
		size = "below average";
		multiplier *= 2;
	}
	else if(size_roll <= 32)
	{
		size = "average";
		multiplier *= 5;
	}
	else if(size_roll <= 40)
	{
		size = "large";
		multiplier *= 10;
	}
	else
	{
		size = "huge";
		multiplier *= 20;
	}

	if(quality_roll <= 4)
	{
		quality = "uncut";
		multiplier *= 5;
		if(magic_roll >= 50)
			magic = "minor magic";
	}
	else if(quality_roll <= 6)
	{
		quality = "rough cut";
		multiplier *= 3;
		if(magic_roll >= 45)
			magic = "minor magic";
	}
	else if(quality_roll <= 8)
	{
		quality = "poor cut";
		multiplier *= .5;
		if(magic_roll >= 40)
			magic = "minor magic";
	}
	else if(quality_roll <= 12)
	{
		quality = "below average cut";
		multiplier *= 1;
		if(magic_roll >= 40)
			magic = "minor magic";
	}
	else if(quality_roll <= 18)
	{
		quality = "standard cut";
		multiplier *= 2;
		if(magic_roll >= 30)
			magic = "average magic";
	}
	else if(quality_roll <= 24)
	{
		quality = "above average cut";
		multiplier *= 5;
		if(magic_roll >= 25)
			magic = "average magic";
	}
	else if(quality_roll <= 34)
	{
		quality = "exceptional cut";
		multiplier *= 10;
		if(magic_roll >= 20)
			magic = "average magic";
	}
	else if(quality_roll <= 42)
	{
		quality = "perfect cut";
		multiplier *= 20;
		if(magic_roll >= 15)
			magic = "major magic";
	}
	else
	{
		quality = "unique cut";
		multiplier *= 50;
		if(magic_roll >= 10)
			magic = "great magic";
	}

	if(type_roll <= 10)
		type = generate_ornamental(&base_value);
	else if(type_roll <= 18)
		type = generate_semiprecious(&base_value);
	else if(type_roll <= 28)
		type = generate_fancy(&base_value);
	else if(type_roll <= 38)
		type = generate_gemstone(&base_value);
	else
		type = generate_jewel(&base_value);

	if(setting_roll <= 5)
		setting = "leather";
	else if(setting_roll <= 10)
		setting = "bronze";
	else if(setting_roll <= 14)
		setting = "iron";
	else if(setting_roll <= 19)
		setting = "silver";
	else if(setting_roll <= 23)
		setting = "gold";
	else if(setting_roll <= 36)
		setting = get_jeweled_item(luck);
	else
		setting = "mithril";

	printf("Found a %s sized %s %s, worth %dsp, it has %s, and is set in %s\n", size, quality, type, (int) (base_value * multiplier), magic, setting);
}

char *generate_ornamental(int *base_value)
{
	int roll = roll_xdy(3, 6);
	char *ret;

	switch(roll)
	{
		case 3: 
			ret = "Azurite (Mottled deep blue)"; break;
		case 4: 
			ret = "Banded Agate (Striped, brown, blue, write, and reddish)"; break;
		case 5: 
			ret = "Blue Quartz (Pale blue)"; break;
		case 6: 
			ret = "Eye agate (Circle pattern of gray, white, brown, blue and/or green)"; break;
		case 7: 
			ret = "Hemetite (Grey and black)"; break;
		case 8: 
			ret = "Lapis Lazuli (Light and dark blue, with yellow flecks)"; break;
		case 9: 
			ret = "Malachite (Striated, light and dark green)"; break;
		case 10: 
			ret = "Moss Agate (Pink and yellow, flecked with grey)"; break;
		case 11: 
			ret = "Obsidian (Black)"; break;
		case 12: 
			ret = "Rhodochryosite (Light pink)"; break;
		case 13: 
			ret = "Tiger Eye (Rich brown with gold hued center)"; break;
		case 14: 
			ret = "Turquoise (Light blue-green)"; break;
		case 15: 
			ret = "Enamal (Red or white)"; break;
		case 16: 
			ret = "Coral (Pink or blue)"; break;
		case 17: 
			ret = "Rock Crystal (Clear)"; break;
		case 18: 
			return generate_semiprecious(base_value);
	}

	*base_value = 25;
	return ret;
}

char *generate_semiprecious(int *base_value)
{
	int roll = roll_xdy(3, 6);
	char *ret;

	switch(roll)
	{
		case 3:
		    ret = "Bloodstone (Dark red)"; break; 
		case 4:
		    ret = "Carnelian (Orange to reddish brown)"; break; 
		case 5:
		    ret = "Sard (Yellow)"; break; 
		case 6:
		    ret = "Chalcedony (White)"; break; 
		case 7:
		    ret = "Chrysoprase (Apple green to emerald green)"; break; 
		case 8:
		    ret = "Citrine (Pale yellowish brown)"; break; 
		case 9:
		    ret = "Jaspar (Blue, black, to brown)"; break; 
		case 10:
		    ret = "Moonstone (White to pale blue)"; break; 
		case 11:
		    ret = "Onyx (Bands of black and white or pure black or white)"; break; 
		case 12:
		    ret = "Sardonyx (Bands of sard and onyx)"; break; 
		case 13:
		    ret = "Cairngorm (Pale blue)"; break; 
		case 14:
		    ret = "Smokey Quartz (Dark grey)"; break; 
		case 15:
		    ret = "Star Rose Quartz (Rosy pink, white, star at center)"; break; 
		case 16:
		    ret = "Iircon (Clear pale blue green)"; break; 
		case 17:
		    ret = "Amyethyst (Greenish-white)"; break; 
		case 18:
		    return generate_fancy(base_value);
	}

	*base_value = 100;

	return ret;
}

char *generate_fancy(int *base_value)
{
	int roll = roll_xdy(3, 6);
	char *ret;

	switch(roll)
	{
		case 3:
			ret = "Amber (Watery to rich gold)"; break; 
		case 4:
			ret = "Alexandrite (Dark green)"; break; 
		case 5:
			ret = "Beryl (Pale green)"; break; 
		case 6:
			ret = "Aquamarine (Pale blue-green)"; break; 
		case 7:
			ret = "Chrysoberyl (Yellow-green)"; break; 
		case 8:
			ret = "Red Coral (Pale red)"; break; 
		case 9:
			ret = "Garnet (Red)"; break; 
		case 10:
			ret = "Jade (Yellow, red, white, or green)"; break; 
		case 11:
			ret = "Jet (Deep black)"; break; 
		case 12:
			ret = "Black Pearl (Black)"; break; 
		case 13:
			ret = "White Pearl (White)"; break; 
		case 14:
			ret = "Peridot (Olive green)"; break; 
		case 15:
			ret = "Spindel (Deep blue)"; break; 
		case 16:
			ret = "Topaz (Golden yellow)"; break; 
		case 17:
			ret = "Tourmaline (Pale brown to pale red)"; break; 
		case 18:
			return generate_gemstone(base_value);
	}

	*base_value = 500;
	return ret;
}

char *generate_gemstone(int *base_value)
{
	int roll = roll_xdy(3, 6);
	char *ret;

	switch(roll)
	{
		case 3:
			ret = "Ivory (Pale white to milky)"; break; 
		case 4:
			ret = "Opal (Black, red, or green)"; break; 
		case 5:
			ret = "Serpentine (Green to grey (rarely sand colored))"; break; 
		case 6:
			ret = "Fire Opal (Fiery red)"; break; 
		case 7:
			ret = "Black Sapphire (Black)"; break; 
		case 8:
			ret = "Sapphire (Blue)"; break; 
		case 9:
			ret = "Diamond (Clear blue-white)"; break; 
		case 10:
			ret = "Iacinth (Fiery orange)"; break; 
		case 11:
			ret = "Ruby (Red, lustrious)"; break; 
		case 12:
			ret = "Oriental Amethyst (Rich purple)"; break; 
		case 13:
			ret = "Oriental Emerald (Clear bright green)"; break; 
		case 14:
			ret = "Oriental Topaz (Fiery yellow)"; break; 
		case 15:
			ret = "Star Ruby (Translucent red with white star center)"; break; 
		case 16:
			ret = "Star Sapphire (Translucent blue with black star center)"; break; 
		case 17:
			ret = "Rainbow Pearl (All colors)"; break; 
		case 18:
			return generate_jewel(base_value);
	}

	*base_value = 1000;

	return ret;
}

char *generate_jewel(int *base_value)
{
	int roll = roll_xdy(3, 6);
	char *ret;

	switch(roll)
	{
		case 3:
			ret = "Rainbow Diamond (All colors)"; break; 
		case 4:
			ret = "Stars' Tear (Pale gold)"; break; 
		case 5:
			ret = "Sun Drop (Fiery yellow)"; break; 
		case 6:
			ret = "Thunderstone (Streaky white)"; break; 
		case 7:
			ret = "Fire Star (Red, orange, or yellow)"; break; 
		case 8:
			ret = "Dune Star (Tan (sandy))"; break; 
		case 9:
			ret = "Dragon Tear (Clear when held, otherwise red)"; break; 
		case 10:
			ret = "Nacre (Yellow)"; break; 
		case 11:
			ret = "Triton Eye (Streaky red with green dots)"; break; 
		case 12:
			ret = "Snow Drop (Milk white)"; break; 
		case 13:
			ret = "Heart Stone (Blood red)"; break; 
		case 14:
			ret = "Dwarf Node (Black)"; break; 
		case 15:
			ret = "Elf Stone (Blue)"; break; 
		case 16:
			ret = "Blood Amber (Red)"; break; 
		case 17:
			ret = "Marmochand (Intense green)"; break; 
		case 18:
			ret = "Tears of Allah (Orange)"; break; 
	}
  
	*base_value = 5000;
	return ret;
}


static char *get_jeweled_item(int luck)
{
	int roll = roll_2d6daro() + roll_level(luck);
	int magic_roll = roll_2d6daro() + roll_level(luck);

	if(roll <= 6)
		return magic_roll >= 15 ? "a magic ring" : "a ring";
	else if(roll <= 10)
		return magic_roll >= 12 ? "a magic bracelet" : "a bracelet";
	else if(roll <= 13)
		return magic_roll >= 10 ? "a magic belt" : "a belt";
	else if(roll <= 17)
		return magic_roll >= 8 ? "a magic necklace" : "a necklace";
	else if(roll <= 21)
		return get_jeweled_weapon(luck, magic_roll >= 7);
	else
		return magic_roll >= 6 ? "magic headgear" : "headgear";
	
}

static char *get_jeweled_weapon(int luck, int magical)
{
	int roll = roll_2d6daro() + roll_level(luck);

	if(roll <= 5)
		return magical ? "a magic dagger" : "a dagger";
	else if(roll <= 9)
		return magical ? "a magic sword" : "a sword";
	else if(roll <= 14)
		return magical ? "a magic hafted weapon" : "a hafted weapon";
	else if(roll <= 18)
		return magical ? "a magic spear" : "a spear";
	else if(roll <= 21)
		return magical ? "a magic polearm" : "a polearm";
	else
		return magical ? "a magic projectile weapon" : "a projectile weapon";
}



