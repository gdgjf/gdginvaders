#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Layer::init() )
  {
      return false;
  }
  
  Size visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  
  LayerColor* background = LayerColor::create(Color4B::WHITE);
  this->addChild(background);

  Vec2 shipPosition = Vec2(visibleSize.width / 2, 100);
  
  Sprite* ship = Sprite::create("gdglogo.png");
  ship->setPosition(shipPosition);
  this->addChild(ship);
  
  Node* monsters = Node::create();
  monsters->setPosition(Vec2(300, 500));
  
  Sprite* appleMonster = Sprite::create("apple.png");
  monsters->addChild(appleMonster);
  
  Sprite* androidMonster = Sprite::create("android.png");
  androidMonster->setPosition(Vec2(200, 0));
  monsters->addChild(androidMonster);
  
  this->addChild(monsters);
  
  MoveBy* moveRight = MoveBy::create(1, Vec2(300, 0));
  MoveBy* moveLeft = moveRight->reverse();
  
  Sequence* move = Sequence::create(moveRight, moveLeft, NULL);
  Repeat* repeat = Repeat::create(move, 2);
  
  MoveBy* moveDown = MoveBy::create(0.3, Vec2(0, -100));
  
  Sequence* moveFewTimesAndMoveDown = Sequence::create(repeat, moveDown, NULL);
  
  Repeat* repeat3 = Repeat::create(moveFewTimesAndMoveDown, 3);
  CallFunc* gameOverFunc = CallFunc::create([=] {
    log("GAME OVER!!!!");
    background->setColor(Color3B::RED);
  });
  
  Sequence* monstersAnimation = Sequence::create(repeat3, gameOverFunc, NULL);
  
  monsters->runAction(monstersAnimation);

  
  auto touchListener = EventListenerTouchOneByOne::create();
  touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
    auto touchPosition = touch->getLocation();
    auto position = Vec2(touchPosition.x, ship->getPosition().y);
    
    auto followTouch = MoveTo::create(1, position);
    
    ship->runAction(followTouch);
    
    return true;
  };
  touchListener->onTouchMoved = [](Touch* touch, Event* event) {
    log("onTouchEnded");
  };
  touchListener->onTouchEnded = [](Touch* touch, Event* event) {
    log("onTouchEnded");
  };
  
  auto dispatcher = Director::getInstance()->getEventDispatcher();
  dispatcher->addEventListenerWithSceneGraphPriority(
      touchListener, this);
  
  return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
