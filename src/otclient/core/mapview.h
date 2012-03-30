/*
 * Copyright (c) 2010-2012 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "declarations.h"
#include <framework/graphics/declarations.h>
#include <framework/luascript/luaobject.h>
#include <framework/core/declarations.h>

class MapView : public LuaObject
{
    enum {
        // 3840x2160 => 1080p optimized
        // 2560x1440 => 720p optimized
        // 1728x972 => 480p optimized
        DEFAULT_FRAMBUFFER_WIDTH = 2560,
        DEFAULT_FRAMBUFFER_HEIGHT = 1440,

        NEAR_VIEW_AREA = 32*32,
        MID_VIEW_AREA = 64*64,
        FAR_VIEW_AREA = 128*128,
        MAX_TILE_UPDATES = NEAR_VIEW_AREA*7
    };

    enum ViewRange {
        NEAR_VIEW,
        MID_VIEW,
        FAR_VIEW,
        HUGE_VIEW
    };

public:
    MapView();
    void draw(const Rect& rect);

private:
    void updateVisibleTilesCache(int start = 0);
    void requestVisibleTilesCacheUpdate() { m_mustUpdateVisibleTilesCache = true; }

protected:
    void onTileUpdate(const Position& pos);

    friend class Map;

public:
    void lockFirstVisibleFloor(int firstVisibleFloor);
    void unlockFirstVisibleFloor();
    void followCreature(const CreaturePtr& creature);

    void setCameraPosition(const Position& pos);
    void setVisibleDimension(const Size& visibleDimension);
    void setAnimated(bool animated) { m_animated = animated; }

    //void zoomIn(float factor);
    //void zoomOut(float factor);

    bool isFollowingCreature() { return !!m_followingCreature; }

    int calcFirstVisibleFloor();
    int calcLastVisibleFloor();
    Position getCameraPosition();
    TilePtr getTile(const Point& mousePos, const Rect& mapRect);
    Size getVisibleDimension() { return m_visibleDimension; }
    Size getVisibleSize() { return m_visibleDimension * m_tileSize; }
    CreaturePtr getFollowingCreature() { return m_followingCreature; }

    bool getViewRange() { return m_viewRange; }
    bool isAnimated() { return m_animated; }

    Point transformPositionTo2D(const Position& position);

    MapViewPtr asMapView() { return std::static_pointer_cast<MapView>(shared_from_this()); }

private:
    int m_lockedFirstVisibleFloor;
    int m_cachedFirstVisibleFloor;
    int m_cachedLastVisibleFloor;
    int m_tileSize;
    Size m_drawDimension;
    Size m_visibleDimension;
    Point m_virtualCenterOffset;
    Point m_visibleCenterOffset;
    Position m_customCameraPosition;
    Boolean<true> m_mustUpdateVisibleTilesCache;
    Boolean<true> m_mustDrawVisibleTilesCache;
    Boolean<true> m_mustCleanFramebuffer;
    Boolean<true> m_animated;
    std::vector<TilePtr> m_cachedVisibleTiles;
    std::vector<CreaturePtr> m_cachedFloorVisibleCreatures;
    EventPtr m_updateTilesCacheEvent;
    CreaturePtr m_followingCreature;
    FrameBufferPtr m_framebuffer;
    PainterShaderProgramPtr m_shaderProgram;
    ViewRange m_viewRange;
};

#endif