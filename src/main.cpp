#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

float btnWidth = 180;
float smallbtnWidth = 120;
float btnHeight = 23;

void formatScale9Sprite(CCScale9Sprite* sprite, bool isSmall)
{
	if (isSmall) sprite->setContentSize(CCSize{ smallbtnWidth, btnHeight });
	else         sprite->setContentSize(CCSize{ btnWidth, btnHeight });
}

// dont really know how to make a button open the mods list
// so this is the best workaround
class FakeLayer : public CCLayer {
public:
	void onModList(CCObject* sender)
	{
		geode::openModsList();
	}
};

class $modify(MenuLayer) {
	static void onModify(auto & self)
	{
		// run after everything so as to not cause anything to be weird
		self.setHookPriority("MenuLayer::init", -6900);
	}

	bool init()
	{
		if (!MenuLayer::init()) return false;

		// I would just make these not visible to keep mods compatible but this is on a really low priority anyway so it shouldn't 
		// fuck with anything else

		if (auto p = typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("play-button")))		p->removeFromParent();
		if (auto p = typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("icon-kit-button"))) p->removeFromParent();
		if (auto p = typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("editor-button")))	p->removeFromParent();

		if (auto p = typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("settings-button")))	p->removeFromParent();
		if (auto p = typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("close-button")))	p->removeFromParent();
		if (auto p = typeinfo_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("geode.loader/geode-button")))
		{
			// if I just remove this, the game will crash when trying to add the little blue dot thingymabob
			// with zero explanation as to why
			// so we have to copy it, move it to the outer layer, then set it to not be visible

			p->removeFromParent();
			this->addChild(p);
			p->setVisible(false);
		}

		if (auto btm = typeinfo_cast<CCMenu*>(this->getChildByID("bottom-menu")))	btm->updateLayout();
		if (auto clm = typeinfo_cast<CCMenu*>(this->getChildByID("close-menu")))	clm->updateLayout();
		
		// TODO: minecraft style buttons
		// if (Mod::get()->getSettingValue(""))
		// pad the play and editor buttons with spaces to make the clickable area larger than it actually is
		auto playSprite = ButtonSprite::create("     Play     ");
		auto editorSprite = ButtonSprite::create("     Editor     ");
		auto iconsSprite = ButtonSprite::create("Icon Kit");
		auto modsSprite = ButtonSprite::create("Mods");
		auto optionsSprite = ButtonSprite::create("Options");
		auto quitSprite = ButtonSprite::create("Quit Game");

		

		auto playButton = CCMenuItemSpriteExtra::create(playSprite, this, menu_selector(MenuLayer::onPlay));
		auto editorButton = CCMenuItemSpriteExtra::create(editorSprite, this, menu_selector(MenuLayer::onCreator));
		auto iconsButton = CCMenuItemSpriteExtra::create(iconsSprite, this, menu_selector(MenuLayer::onGarage));
		auto modsButton = CCMenuItemSpriteExtra::create(modsSprite, this, menu_selector(FakeLayer::onModList));
		auto optionsButton = CCMenuItemSpriteExtra::create(optionsSprite, this, menu_selector(MenuLayer::onOptions));
		auto quitButton = CCMenuItemSpriteExtra::create(quitSprite, this, menu_selector(MenuLayer::onQuit));

		float scale = 0.675;

		// change the width + height so they're all the same
		if (auto ccscale9sprite = typeinfo_cast<CCScale9Sprite*>(playSprite->getChildren()->objectAtIndex(1)))		formatScale9Sprite(ccscale9sprite, false);
		if (auto ccscale9sprite = typeinfo_cast<CCScale9Sprite*>(editorSprite->getChildren()->objectAtIndex(1)))	formatScale9Sprite(ccscale9sprite, false);
		if (auto ccscale9sprite = typeinfo_cast<CCScale9Sprite*>(iconsSprite->getChildren()->objectAtIndex(1)))		formatScale9Sprite(ccscale9sprite, true);
		if (auto ccscale9sprite = typeinfo_cast<CCScale9Sprite*>(modsSprite->getChildren()->objectAtIndex(1)))		formatScale9Sprite(ccscale9sprite, true);
		if (auto ccscale9sprite = typeinfo_cast<CCScale9Sprite*>(optionsSprite->getChildren()->objectAtIndex(1)))	formatScale9Sprite(ccscale9sprite, true);
		if (auto ccscale9sprite = typeinfo_cast<CCScale9Sprite*>(quitSprite->getChildren()->objectAtIndex(1)))		formatScale9Sprite(ccscale9sprite, true);

		if (auto label = typeinfo_cast<CCLabelBMFont*>(playSprite->getChildren()->objectAtIndex(0)))	label->setScale(scale);
		if (auto label = typeinfo_cast<CCLabelBMFont*>(editorSprite->getChildren()->objectAtIndex(0)))	label->setScale(scale);
		if (auto label = typeinfo_cast<CCLabelBMFont*>(iconsSprite->getChildren()->objectAtIndex(0)))	label->setScale(scale);
		if (auto label = typeinfo_cast<CCLabelBMFont*>(modsSprite->getChildren()->objectAtIndex(0)))	label->setScale(scale);
		if (auto label = typeinfo_cast<CCLabelBMFont*>(optionsSprite->getChildren()->objectAtIndex(0)))	label->setScale(scale);
		if (auto label = typeinfo_cast<CCLabelBMFont*>(quitSprite->getChildren()->objectAtIndex(0)))	label->setScale(scale);

		if (auto mainMenu = typeinfo_cast<CCMenu*>(this->getChildByIDRecursive("main-menu")))
		{
			mainMenu->addChild(playButton);
			mainMenu->addChild(editorButton);

			// place these next to each other
			auto modAndIconMenu = CCMenu::create();
			modAndIconMenu->setLayout(AxisLayout::create());
			if (auto rawLayout = modAndIconMenu->getLayout())
				if (auto layout = typeinfo_cast<AxisLayout*>(rawLayout))
				{
					layout->setAxisAlignment(AxisAlignment::Even);
					layout->setGap(69); // this fixes some random stuff with spacing i dont know I really need to rethink these ccmenus
				}

			modAndIconMenu->addChild(modsButton);
			modAndIconMenu->addChild(iconsButton);
			modAndIconMenu->setContentSize(CCSize{ btnWidth + 15, modAndIconMenu->getContentSize().height});
			modAndIconMenu->updateLayout();
			mainMenu->addChild(modAndIconMenu);

			auto optionsAndQuitMenu = CCMenu::create();
			optionsAndQuitMenu->setLayout(AxisLayout::create());
			if (auto rawLayout = optionsAndQuitMenu->getLayout())
				if (auto layout = typeinfo_cast<AxisLayout*>(rawLayout))
				{
					layout->setAxisAlignment(AxisAlignment::Even);
					layout->setGap(8); // more random spacing fuckery
				}

			optionsAndQuitMenu->addChild(optionsButton);
			optionsAndQuitMenu->addChild(quitButton);
			optionsAndQuitMenu->setContentSize(CCSize{ btnWidth + 15, optionsAndQuitMenu->getContentSize().height });
			optionsAndQuitMenu->updateLayout();
			optionsAndQuitMenu->setPositionY(optionsAndQuitMenu->getPositionY() - 0.920f); // add more padding above it
			mainMenu->addChild(optionsAndQuitMenu);

			mainMenu->setPositionY(218);
			mainMenu->setAnchorPoint(CCPoint{ 0.5, 1 });
			mainMenu->setScale(Mod::get()->getSettingValue<double>("scale"));
			 
			if (auto rawLayout = mainMenu->getLayout()) {
				if (auto layout = typeinfo_cast<AxisLayout*>(rawLayout)) {
					layout->setAxis(Axis::Column);
					layout->setAxisAlignment(AxisAlignment::End);
					layout->setAxisReverse(true); // why??
					layout->setGap(-4);
					layout->setAutoScale(false);
				}
			}

			mainMenu->updateLayout();
		}

		return true;
	}
};
