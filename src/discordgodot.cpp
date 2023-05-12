#include "discordgodot.h"
#include "lib/discord_game_sdk/cpp/discord.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/time.hpp>

using namespace godot;

discord_sdk *discord_sdk::singleton = nullptr;

discord::Core *core{};
discord::Result result;
discord::Activity activity{};
discord::User user{};

void discord_sdk::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("debug"), &discord_sdk::debug);
    ClassDB::bind_method(D_METHOD("coreupdate"), &discord_sdk::coreupdate);

    ClassDB::bind_method(D_METHOD("get_app_id"), &discord_sdk::get_app_id);
    ClassDB::bind_method(D_METHOD("set_app_id", "app_id"), &discord_sdk::set_app_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "app_id"), "set_app_id", "get_app_id");
    ClassDB::bind_method(D_METHOD("get_state"), &discord_sdk::get_state);
    ClassDB::bind_method(D_METHOD("set_state", "state"), &discord_sdk::set_state);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "state"), "set_state", "get_state");
    ClassDB::bind_method(D_METHOD("get_details"), &discord_sdk::get_details);
    ClassDB::bind_method(D_METHOD("set_details", "details"), &discord_sdk::set_details);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "details"), "set_details", "get_details");

    ClassDB::bind_method(D_METHOD("get_large_image"), &discord_sdk::get_large_image);
    ClassDB::bind_method(D_METHOD("set_large_image", "large_image"), &discord_sdk::set_large_image);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "large_image"), "set_large_image", "get_large_image");
    ClassDB::bind_method(D_METHOD("get_large_image_text"), &discord_sdk::get_large_image_text);
    ClassDB::bind_method(D_METHOD("set_large_image_text", "large_image_text"), &discord_sdk::set_large_image_text);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "large_image_text"), "set_large_image_text", "get_large_image_text");
    ClassDB::bind_method(D_METHOD("get_small_image"), &discord_sdk::get_small_image);
    ClassDB::bind_method(D_METHOD("set_small_image", "small_image"), &discord_sdk::set_small_image);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "small_image"), "set_small_image", "get_small_image");
    ClassDB::bind_method(D_METHOD("get_small_image_text"), &discord_sdk::get_small_image_text);
    ClassDB::bind_method(D_METHOD("set_small_image_text", "large_small_text"), &discord_sdk::set_small_image_text);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "small_image_text"), "set_small_image_text", "get_small_image_text");

    ClassDB::bind_method(D_METHOD("get_start_timestamp"), &discord_sdk::get_start_timestamp);
    ClassDB::bind_method(D_METHOD("set_start_timestamp", "start_timestamp"), &discord_sdk::set_start_timestamp);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "start_timestamp"), "set_start_timestamp", "get_start_timestamp");
    ClassDB::bind_method(D_METHOD("get_end_timestamp"), &discord_sdk::get_end_timestamp);
    ClassDB::bind_method(D_METHOD("set_end_timestamp", "end_timestamp"), &discord_sdk::set_end_timestamp);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "end_timestamp"), "set_end_timestamp", "get_end_timestamp");

    ClassDB::bind_method(D_METHOD("refresh"), &discord_sdk::refresh);

    ClassDB::bind_method(D_METHOD("get_is_discord_working"), &discord_sdk::get_is_discord_working);

    ClassDB::bind_method(D_METHOD("get_result_int"), &discord_sdk::get_result_int);
}

discord_sdk *discord_sdk::get_singleton()
{
    return singleton;
}

discord_sdk::discord_sdk()
{
    ERR_FAIL_COND(singleton != nullptr);
    singleton = this;
}

discord_sdk::~discord_sdk()
{
    ERR_FAIL_COND(singleton != this);
    singleton = nullptr;
}

void discord_sdk::coreupdate()
{
    if (result == discord::Result::Ok && app_id > 0)
    {
        ::core->RunCallbacks();
    }
}
void discord_sdk::debug()
{
    result = discord::Core::Create(1080224638845591692, DiscordCreateFlags_NoRequireDiscord, &core);
    activity.SetState("Test from Godot!");
    activity.SetDetails("I worked months on this");
    activity.GetAssets().SetLargeImage("test1");
    activity.GetAssets().SetLargeText("wow test text for large image");
    activity.GetAssets().SetSmallImage("godot");
    activity.GetAssets().SetSmallText("wow test text for small image");
    activity.GetTimestamps().SetStart(1682242800);
    if (result == discord::Result::Ok)
    {
        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
        core->UserManager().OnCurrentUserUpdate.Connect([]()
                                                        { core->UserManager().GetCurrentUser(&user); });
    }
    else
        UtilityFunctions::push_warning("Discord Activity couldn't be updated. It could be that Discord isn't running!");
}

void discord_sdk::set_app_id(int64_t value)
{
    app_id = value;
    result = discord::Core::Create(value, DiscordCreateFlags_NoRequireDiscord, &core);
}
int64_t discord_sdk::get_app_id()
{
    return app_id;
}

void discord_sdk::set_state(String value)
{
    state = value;
    activity.SetState(value.utf8().get_data());
}
String discord_sdk::get_state()
{
    return state;
}
void discord_sdk::set_details(String value)
{
    details = value;
    activity.SetDetails(value.utf8().get_data());
}
String discord_sdk::get_details()
{
    return details;
}

void discord_sdk::refresh()
{
    if (result == discord::Result::Ok && app_id > 0)
    {
        core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
        core->UserManager().OnCurrentUserUpdate.Connect([]()
                                                        { core->UserManager().GetCurrentUser(&user); });
    }
    else
        UtilityFunctions::push_warning("Discord Activity couldn't be updated. It could be that Discord isn't running!");
}

void discord_sdk::set_large_image(String value)
{
    large_image = value;
    activity.GetAssets().SetLargeImage(value.utf8().get_data());
}
String discord_sdk::get_large_image()
{
    return large_image;
}
void discord_sdk::set_large_image_text(String value)
{
    large_image_text = value;
    activity.GetAssets().SetLargeText(value.utf8().get_data());
}
String discord_sdk::get_large_image_text()
{
    return large_image_text;
}
void discord_sdk::set_small_image(String value)
{
    small_image = value;
    activity.GetAssets().SetSmallImage(value.utf8().get_data());
}
String discord_sdk::get_small_image()
{
    return small_image;
}
void discord_sdk::set_small_image_text(String value)
{
    small_image_text = value;
    activity.GetAssets().SetSmallText(value.utf8().get_data());
}
String discord_sdk::get_small_image_text()
{
    return small_image_text;
}

void discord_sdk::set_start_timestamp(int64_t value)
{
    start_timestamp = value;
    activity.GetTimestamps().SetStart(value);
}
int64_t discord_sdk::get_start_timestamp()
{
    return activity.GetTimestamps().GetStart();
}
void discord_sdk::set_end_timestamp(int64_t value)
{
    end_timestamp = value;
    activity.GetTimestamps().SetEnd(value);
}
int64_t discord_sdk::get_end_timestamp()
{
    return activity.GetTimestamps().GetEnd();
}

bool discord_sdk::get_is_discord_working()
{
    return result == discord::Result::Ok && app_id > 0;
}

int discord_sdk::get_result_int()
{
    return static_cast<int>(result);
}
