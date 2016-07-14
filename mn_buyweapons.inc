#if  defined MN_BUY_WEAPONS_INCLUDED_
	#endinput
#endif 
#define MN_BUY_WEAPONS_INCLUDED_

#if !defined MAX_WEAPON_NAME
	#define MAX_WEAPON_NAME (19)
#endif

#if  !defined DIALOG_WEAPONS_ID
	#define DIALOG_WEAPONS_ID 18498
#endif

#if  !defined DIALOG_PROOF_OF_PURCHASE
	#define DIALOG_PROOF_OF_PURCHASE DIALOG_WEAPONS_ID+1
#endif 

static
	mn_weaponid[MAX_PLAYERS char],
	mn_ammunition[MAX_PLAYERS char],
	mn_slot[MAX_PLAYERS char],
	mn_price[MAX_PLAYERS],
	mn_old_weapon_name[MAX_WEAPON_NAME],
	mn_new_weapon_name[MAX_WEAPON_NAME];

static const
	mn_buystat_caliber[13][] =
    	{
        	{!".45"},
        	{!".45"},
        	{!".50"},
        	{!"12-й"},
        	{!"12-й"},
        	{!"12-й"},
        	{!"9x19 мм Парабеллум"},
        	{!"9x19 мм Парабеллум"},
		{!"7,62 мм"},
        	{!"5,56x45 мм НАТО"},
        	{!"9x19 мм Парабеллум"},
        	{!"7,62 мм"},
        	{!"7,62 мм"}
    	},
    	mn_buystat_shooting_range[][] =
    	{
        	{!"30-35 метров"},
        	{!"30-35 метров"},
        	{!"30-35 метров"},
        	{!"40 метров"},
        	{!"30-35 метров"},
        	{!"40 метров"},
        	{!"30-35 метров"},
        	{!"40-45 метров"},
        	{!"70 метров"},
        	{!"90 метров"},
        	{!"30-35 метров"},
        	{!"100 метров"},
        	{!"100 метров"}
    	},
    	mn_weapon_slot_id[] = 
    	{ 
        	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 4, 6, 6
    	};

stock BuyWeapons(playerid, weaponid)
{
	const 
        	BUY_INFO = 35;
    	mn_weaponid{playerid} = weaponid; 
    	weaponid -= 22;
    	GetWeaponInfo(playerid);
    	GetWeaponName(mn_weaponid{playerid}, mn_new_weapon_name, sizeof(mn_new_weapon_name));
	new 
		string[6], 
        	str[BUY_INFO] = !"Оружие:\t\t\t";
	strcat(str, mn_new_weapon_name);
	strcat(str, !"\n\nКалибр:\t\t\t");
	strcat(str, mn_buystat_caliber[weaponid]);
	strcat(str, !"\nПатронов в обойме:\t\t");
	format(string, sizeof string, "%i", mn_ammunition{playerid});
	strcat(str, string);
	strcat(str, !"\nДальность стрельбы:\t\t");
	strcat(str, mn_buystat_shooting_range[weaponid]);
	strcat(str, !"\nЦена:\t\t\t\t$");
	format(string, sizeof string, "%i", mn_price[playerid]);
	strcat(str, string);
	ShowPlayerDialog(playerid, DIALOG_WEAPONS_ID, DIALOG_STYLE_MSGBOX, "Покупка оружия", str, "Купить", "Закрыть");
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (dialogid == DIALOG_WEAPONS_ID)
	{
	        new
			mn_player_slot[7],
			mn_player_weapons[7],
			mn_player_ammo[7];
	        if (0 == response)
			return SendClientMessage(playerid, 0xAA3333AA, !"Покупка отклонена");
		GetWeaponName(mn_weaponid{playerid}, mn_new_weapon_name, sizeof mn_new_weapon_name);
		for (new i = 2; i < 7; i++)
	        {
	            	GetPlayerWeaponData(playerid, i, mn_player_weapons[i], mn_player_ammo[i]);
	            	mn_player_slot[i] = mn_weapon_slot_id[mn_player_weapons[i]];
	            	if (mn_player_weapons[i] == mn_weaponid{playerid} && mn_player_ammo[i] != 0)
	                	return SendClientMessage(playerid, 0xAA3333AA, !"У Вас уже есть это оружие");
	            	if (mn_player_slot[i] == mn_slot{playerid} && mn_player_weapons[i] != mn_weaponid{playerid} && mn_player_ammo[i] != 0)
			{
				const 
					RES = 36;
				GetWeaponName(mn_player_weapons[i], mn_old_weapon_name, sizeof mn_old_weapon_name);
				new
					str[RES] = !"У Вас уже есть оружие в этом слоте\nПри подтверждении все патроны старого оружия удалятся\n\nЗаменить ";
                		strcat(str, mn_old_weapon_name);
                		strcat(str, !" на ");
                		strcat(str, mn_new_weapon_name);
                		strcat(str, !"?");
                		return ShowPlayerDialog(playerid, DIALOG_PROOF_OF_PURCHASE, DIALOG_STYLE_MSGBOX, "Внимание!", str, "Да", "Закрыть");
	            	}
	        }
        	Buy(playerid);
		return 1;
	}
    	else if (dialogid == DIALOG_PROOF_OF_PURCHASE)
    	{
        	if (0 == response)
			return SendClientMessage(playerid, 0xAA3333AA, !"Покупка отклонена");
        	Buy(playerid);
        	return 1;
    	}
#if  defined mn_bw__OnDialogResponse
    	mn_bw__OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
#endif 
    	return 0;
}
#if  defined _ALS_OnDialogResponse
    	#undef OnDialogResponse
#else
    	#define _ALS_OnDialogResponse
#endif 
#define OnDialogResponse mn_bw__OnDialogResponse
#if  defined mn_bw__OnDialogResponse
forward mn_bw__OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif 

public OnPlayerDisconnect(playerid, reason)
{
    	mn_weaponid{playerid} =
    	mn_ammunition{playerid} =
    	mn_price[playerid] =
    	mn_slot{playerid} = 0;
#if  defined mn_bw__OnPlayerDisconnect
    	mn_bw__OnPlayerDisconnect(playerid, reason);
#endif 
    	return 1;
}
#if  defined _ALS_OnPlayerDisconnect
    	#undef OnPlayerDisconnect
#else
    	#define _ALS_OnPlayerDisconnect
#endif 
#define OnPlayerDisconnect mn_bw__OnPlayerDisconnect
#if  defined mn_bw__OnPlayerDisconnect
forward mn_bw__OnPlayerDisconnect(playerid, reason);
#endif 

static stock GetWeaponInfo(playerid)
{
	static 
		weaponid;
	static const
		slot[] =
		{
			2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 4, 6, 6
		},
		ammunition[] = 
		{
			17, 17, 7, 1, 2, 7, 50, 30, 30, 50, 50, 1, 1
		},
		price[] = //Цены на оружие
		{
			1720, //9mm
			2135, //Silenced 9mm
            		2476, //Desert Eagle
            		1459, //Shotgun
			1928, //Sawnoff Shotgun
            		2836, //Combat Shotgun
            		1838, //Micro SMG/Uzi
            		2382, //MP5
            		3718, //AK-47
            		3967, //M4
            		2593, //Tec-9
            		4391, //Country Rifle
            		5628 //Sniper Rifle
        	};
	weaponid = mn_weaponid{playerid}-22;                                                                                                                    
    	mn_slot{playerid} = slot[weaponid];
    	mn_ammunition{playerid} = ammunition[weaponid];
    	mn_price[playerid] = price[weaponid];
	return 1;
}

static stock Buy(playerid)
{
	if (GetPlayerMoney(playerid) < mn_price[playerid]) 
		return SendClientMessage(playerid, 0xAA3333AA, !"У Вас недостаточно средств");
	GivePlayerMoney(playerid, -mn_price[playerid]);
	GivePlayerWeapon(playerid, mn_weaponid{playerid}, 1);
	SetPlayerAmmo(playerid, mn_weaponid{playerid}, 0);
	GivePlayerWeapon(playerid, mn_weaponid{playerid}, mn_ammunition{playerid});
	SendClientMessage(playerid, 0x33AA33AA, !"Вы успешно совершили покупку");
	return 1;
}