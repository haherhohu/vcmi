/*
 * ERM_BM.cpp, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#include "StdInc.h"

#include "../scripting/ScriptFixture.h"
#include "../JsonComparer.h"

#include "../../lib/NetPacks.h"

namespace test
{
namespace scripting
{

using namespace ::testing;

class ERM_BM : public Test, public ScriptFixture
{
protected:
	void SetUp() override
	{
		ScriptFixture::setUp();
	}
};

TEST_F(ERM_BM, GetAttack)
{
	const int32_t ATTACK_VALUE = 345;
	const uint32_t UNIT_ID = 42;

	battle::UnitFake & unit1 = unitsFake.add(0);
	unit1.addNewBonus(std::make_shared<Bonus>(Bonus::PERMANENT, Bonus::PRIMARY_SKILL, Bonus::CREATURE_ABILITY, ATTACK_VALUE, 0, PrimarySkill::ATTACK));

	EXPECT_CALL(binfoMock, battleGetUnitByID(Eq(UNIT_ID))).WillOnce(Return(&unit1));

	unitsFake.setDefaultBonusExpectations();

	std::stringstream source;
	source << "VERM" << std::endl;
	source << "!?PI;" << std::endl;
	source << "!!BM"<<UNIT_ID<<":A?v1;" << std::endl;

	loadScript(VLC->scriptHandler->erm, source.str());
	SCOPED_TRACE("\n" + subject->code);
	run();

	JsonNode actualState = context->saveState();

	EXPECT_EQ(actualState["ERM"]["v"]["1"].Float(), ATTACK_VALUE);
}

}
}
