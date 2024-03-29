/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int statusallmons      = 1;        /* 0 means no statusbar on all monitors */
static const int focusonwheel       = 0;
static const char *fonts[]          = {
    "Monospace:size=15", "Hack Nerd Font:size=15",
    "Noto Color Emoji:pixelsize=15",
};

/* unfocused bar background color */
static const char normbgcolor[]     = "#222222";
/* unfocused bar foreground color */
static const char normfgcolor[]     = "#DFDFDF";
/* unfocused windows border color */
static const char normbordercolor[] = "#444444";
/* selected bar background color */
static const char selbgcolor[]      = "#444444";
/* selected bar foreground color */
static const char selfgcolor[]      = "#FFFF55";
/* selected window border color */
static const char selbordercolor[]  = "#E60053";
static const char *colors[][3] = {
    /*                  fg          bg          border   */
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = { "st", "-n", "spterm", NULL };
const char *spcmd2[] = { "st", "-n", "splf", "-e", "lf_run", NULL };
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      spcmd1},
    {"splf",        spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8󰇰", "9", "0" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class                instance    title       tags mask     isfloating   monitor */
    { "thunderbird",        NULL,       NULL,       1 << 7,         0,          -1 },
    { "VirtualBox Manager", NULL,       NULL,       1 << 5,         0,          -1 },
    { NULL,                 "spterm",   NULL,       SPTAG(0),       1,          -1 },
    { NULL,                 "splf",     NULL,       SPTAG(1),       1,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.h"

static const Layout layouts[] = {
    /* symbol   arrange function */
    { "[]=",    tile },
    { "TTT",    bstack },
    { "[M]",    monocle },
    { "[D]",    deck },
    { "###",    nrowgrid },
    { "[\\]",   dwindle },
    { "|M|",    centeredmaster },
    { ">M>",    centeredfloatingmaster },
    { "><>",    NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
    { MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
    { MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
    { MOD, XK_n,     ACTION##stack, {.i = 0 } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[] = { "st", NULL };
static const char *roficmd[] = { "rofi", "-monitor", "-4", "-show", "drun", NULL };
static const char *browsercmd[] = { "google-chrome-stable", NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
    /* modifier                     key         function        argument */
    STACKKEYS(MODKEY,                           focus)
    STACKKEYS(MODKEY|ShiftMask,                 push)
    TAGKEYS(                        XK_1,                       0)
    TAGKEYS(                        XK_2,                       1)
    TAGKEYS(                        XK_3,                       2)
    TAGKEYS(                        XK_4,                       3)
    TAGKEYS(                        XK_5,                       4)
    TAGKEYS(                        XK_6,                       5)
    TAGKEYS(                        XK_7,                       6)
    TAGKEYS(                        XK_8,                       7)
    TAGKEYS(                        XK_9,                       8)
    TAGKEYS(                        XK_0,                       9)
    { MODKEY,                       XK_slash,   view,           {.ui = ~0 } },

    /* layouts */
    { MODKEY,                       XK_m,       setlayout,      {.v = &layouts[0]} },   /* tile */
    { MODKEY|ShiftMask,             XK_m,       setlayout,      {.v = &layouts[1]} },   /* bstack */
    { MODKEY,                       XK_i,       setlayout,      {.v = &layouts[2]} },   /* monocle */
    { MODKEY|ShiftMask,             XK_i,       setlayout,      {.v = &layouts[3]} },   /* deck */
    { MODKEY,                       XK_u,       setlayout,      {.v = &layouts[4]} },   /* nrowgrid */
    { MODKEY|ShiftMask,             XK_u,       setlayout,      {.v = &layouts[5]} },   /* dwindle */
    { MODKEY,                       XK_y,       setlayout,      {.v = &layouts[6]} },   /* centeredmaster */
    { MODKEY|ShiftMask,             XK_y,       setlayout,      {.v = &layouts[7]} },   /* centeredfloatingmaster */
    { MODKEY|ShiftMask,             XK_f,       setlayout,      {.v = &layouts[8]} },   /* floating */
    { MODKEY,                       XK_f,       togglefullscr,  {0} },
    { MODKEY,                       XK_s,       togglefloating, {0} },

    /* quit/restart dwm */
    { MODKEY|ShiftMask,             XK_q,       quit,           {0} },
    { MODKEY|ShiftMask,             XK_r,       quit,           {1} },
    /* close window */
    { MODKEY|ShiftMask,             XK_w,       killclient,     {0} },
    /* resize window */
    { MODKEY,                       XK_h,       setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,       setmfact,       {.f = +0.05} },
    /* gaps */
    { MODKEY,                       XK_a,       togglegaps,     {0} },
    { MODKEY|ShiftMask,             XK_a,       defaultgaps,    {0} },
    { MODKEY,                       XK_z,       incrgaps,       {.i = +3 } },
    { MODKEY,                       XK_x,       incrgaps,       {.i = -3 } },
    /* switch between last opened tag */
    { MODKEY,                       XK_Tab,     view,           {0} },
    /* go a tag left/right */
    { MODKEY,               XK_bracketleft,     shiftview,      {.i = -1 } },
    { MODKEY,               XK_bracketright,    shiftview,      {.i = +1 } },
    /* move to left/right tag */
    { MODKEY|ShiftMask,     XK_bracketleft,     shifttag,       {.i = -1 } },
    { MODKEY|ShiftMask,     XK_bracketright,    shifttag,       {.i = +1 } },
    /* promote selected window to master */
    { MODKEY,                       XK_space,   zoom,           {0} },
    /* sticky windows */
    { MODKEY|ShiftMask,             XK_s,       togglesticky,   {0} },
    /* increase/decrease the number of master windows */
    { MODKEY|ShiftMask,             XK_equal,   incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,             XK_minus,   incnmaster,     {.i = -1 } },
    /* interact with other monitors */
    { MODKEY,                       XK_comma,   focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period,  focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,   tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,  tagmon,         {.i = +1 } },
    /* toggle bar */
    { MODKEY,                       XK_b,       togglebar,      {0} },

    /* launch */
    { MODKEY,                       XK_Return,  spawn,          {.v = termcmd } },
    { MODKEY,                       XK_d,       spawn,          {.v = roficmd } },
    { MODKEY,                       XK_w,       spawn,          {.v = browsercmd } },
    { 0,                            XK_Print,   spawn,          SHCMD("flameshot gui") },
    { ShiftMask,                    XK_Print,   spawn,          SHCMD("flameshot full") },
    { MODKEY,                       XK_Escape,  spawn,          SHCMD("powermenu") },
    /* scratchpads */
    { MODKEY|ShiftMask,             XK_Return,  togglescratch,  {.ui = 0 } },
    { MODKEY|ShiftMask,             XK_l,       togglescratch,  {.ui = 1 } },
    /* audio */
    { 0, XF86XK_AudioMute,          spawn,      SHCMD("volume_ctrl toggle; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioRaiseVolume,   spawn,      SHCMD("volume_ctrl up; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioLowerVolume,   spawn,      SHCMD("volume_ctrl down; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioPlay,          spawn,      SHCMD("playerctl play-pause") },
    { 0, XF86XK_AudioPrev,          spawn,      SHCMD("playerctl previous") },
    { 0, XF86XK_AudioNext,          spawn,      SHCMD("playerctl next") },
    /* backlight */
    { 0, XF86XK_MonBrightnessUp,    spawn,      SHCMD("bright_ctrl up") },
    { 0, XF86XK_MonBrightnessDown,  spawn,      SHCMD("bright_ctrl down") },
    /* touchpad */
    { 0, XF86XK_TouchpadToggle,     spawn,      SHCMD("touchpad_ctrl") },
    /* screensaver */
    { 0, XF86XK_PowerOff,           spawn,      SHCMD("xset dpms force off") },
    { MODKEY, XF86XK_PowerOff,      spawn,      SHCMD("systemctl suspend") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {.v = &layouts[0]} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 1} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkClientWin,         MODKEY,         Button4,        incrgaps,       {.i = +1} },
    { ClkClientWin,         MODKEY,         Button5,        incrgaps,       {.i = -1} },
};
