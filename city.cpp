#include "city.h"
#include "mathFunctions.h"

City::City(int xWidth, int yWidth)
{
    this->xWidth = xWidth;
    this->yWidth = yWidth;
    minBlockSize = 3;
    maxBlockSize = 8;
    minBuildingHeight = 0.5;
    maxBuildingHeight = 10000;
    heightMean = 2;
    heightVariance = 0.5;

    blockList = new List<Block*>();
    roadList = new List<Element*>();
}

City::~City()
{
    blockList->start();
    while(!blockList->isAtTheEnd())
    {
        delete blockList->getCurrentElement();
        blockList->next();
    }
    delete blockList;
    roadList->start();
    while(!roadList->isAtTheEnd())
    {
        delete roadList->getCurrentElement();
        roadList->next();
    }
    delete roadList;
}

void City::divideInBlocks(Zone zone)
{
    int xRoad, yRoad;
    bool xDivide=false, yDivide=false;

    if(zone.getXWidth()>maxBlockSize) // Si la zone est plus grande que la taille maximale d'un bloc.
    {

        xRoad=rand(zone.getX1()+minBlockSize+1, zone.getX2()-minBlockSize); // On la d�coupe en prenant soin de ne pas depasser la taille minimale d'un bloc.
        xDivide=true;
    }

    if(zone.getYWidth()>maxBlockSize)
    {
        yRoad=rand(zone.getY1()+minBlockSize+1, zone.getY2()-minBlockSize);
        yDivide=true;
    }


    if(xDivide && yDivide)
    {
        roadList->addElement(new Element(Zone(xRoad,zone.getY1(),xRoad+1, yRoad), 0, X_ROAD));
        roadList->addElement(new Element(Zone(xRoad,yRoad,xRoad+1, yRoad+1), 0, INTERSECTION));
        roadList->addElement(new Element(Zone(xRoad,yRoad+1,xRoad+1, zone.getY2()), 0, X_ROAD));
        roadList->addElement(new Element(Zone(zone.getX1(),yRoad,xRoad, yRoad+1), 0, Y_ROAD));
        roadList->addElement(new Element(Zone(xRoad+1,yRoad,zone.getX2(), yRoad+1), 0, Y_ROAD));

        divideInBlocks(Zone(zone.getX1(),zone.getY1(),xRoad,yRoad));
        divideInBlocks(Zone(xRoad+1,zone.getY1(),zone.getX2(),yRoad));
        divideInBlocks(Zone(zone.getX1(),yRoad+1,xRoad,zone.getY2()));
        divideInBlocks(Zone(xRoad+1,yRoad+1,zone.getX2(),zone.getY2()));
    }
    else if(xDivide)
    {
        roadList->addElement(new Element(Zone(xRoad,zone.getY1(),xRoad+1, zone.getY2()), 0, X_ROAD));
        divideInBlocks(Zone(zone.getX1(),zone.getY1(),xRoad,zone.getY2()));
        divideInBlocks(Zone(xRoad+1,zone.getY1(),zone.getX2(),zone.getY2()));
    }
    else if(yDivide)
    {
        roadList->addElement(new Element(Zone(zone.getX1(),yRoad,zone.getX2(), yRoad+1), 0, Y_ROAD));
        divideInBlocks(Zone(zone.getX1(),zone.getY1(),zone.getX2(),yRoad));
        divideInBlocks(Zone(zone.getX1(),yRoad+1,zone.getX2(),zone.getY2()));
    }
    else
    {
        blockList->addElement(new Block(zone, minBuildingHeight, maxBuildingHeight, heightMean, heightVariance, garden));
    }
}
