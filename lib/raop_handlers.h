/**
 *  Copyright (C) 2018  Juho Vähä-Herttua
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

/* This file should be only included from raop.c as it defines static handler
 * functions and depends on raop internals */

#include "dnssdint.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <plist/plist.h>

typedef void (*raop_handler_t)(raop_conn_t *, http_request_t *,
                               http_response_t *, char **, int *);

static void
raop_handler_info(raop_conn_t *conn,
                  http_request_t *request, http_response_t *response,
                  char **response_data, int *response_datalen)
{
    assert(conn->raop->dnssd);

    int airplay_txt_len = 0;
    const char *airplay_txt = dnssd_get_airplay_txt(conn->raop->dnssd, &airplay_txt_len);

    int name_len = 0;
    const char *name = dnssd_get_name(conn->raop->dnssd, &name_len);

    int hw_addr_raw_len = 0;
    const char *hw_addr_raw = dnssd_get_hw_addr(conn->raop->dnssd, &hw_addr_raw_len);

    char *hw_addr = calloc(1, 3 * hw_addr_raw_len);
    //int hw_addr_len =
    utils_hwaddr_airplay(hw_addr, 3 * hw_addr_raw_len, hw_addr_raw, hw_addr_raw_len);

    int pk_len = 0;
    char *pk = utils_parse_hex(AIRPLAY_PK, strlen(AIRPLAY_PK), &pk_len);

    plist_t r_node = plist_new_dict();

    plist_t txt_airplay_node = plist_new_data(airplay_txt, airplay_txt_len);
    plist_dict_set_item(r_node, "txtAirPlay", txt_airplay_node);

    plist_t features_node = plist_new_uint((uint64_t) 0x1E << 32 | 0x5A7FFFF7);
    plist_dict_set_item(r_node, "features", features_node);

    plist_t name_node = plist_new_string(name);
    plist_dict_set_item(r_node, "name", name_node);

    plist_t audio_formats_node = plist_new_array();
    plist_t audio_format_0_node = plist_new_dict();
    plist_t audio_format_0_type_node = plist_new_uint(100);
    plist_t audio_format_0_audio_input_formats_node = plist_new_uint(0x3fffffc);
    plist_t audio_format_0_audio_output_formats_node = plist_new_uint(0x3fffffc);
    plist_dict_set_item(audio_format_0_node, "type", audio_format_0_type_node);
    plist_dict_set_item(audio_format_0_node, "audioInputFormats", audio_format_0_audio_input_formats_node);
    plist_dict_set_item(audio_format_0_node, "audioOutputFormats", audio_format_0_audio_output_formats_node);
    plist_array_append_item(audio_formats_node, audio_format_0_node);
    plist_t audio_format_1_node = plist_new_dict();
    plist_t audio_format_1_type_node = plist_new_uint(101);
    plist_t audio_format_1_audio_input_formats_node = plist_new_uint(0x3fffffc);
    plist_t audio_format_1_audio_output_formats_node = plist_new_uint(0x3fffffc);
    plist_dict_set_item(audio_format_1_node, "type", audio_format_1_type_node);
    plist_dict_set_item(audio_format_1_node, "audioInputFormats", audio_format_1_audio_input_formats_node);
    plist_dict_set_item(audio_format_1_node, "audioOutputFormats", audio_format_1_audio_output_formats_node);
    plist_array_append_item(audio_formats_node, audio_format_1_node);
    plist_dict_set_item(r_node, "audioFormats", audio_formats_node);

    plist_t pi_node = plist_new_string(AIRPLAY_PI);
    plist_dict_set_item(r_node, "pi", pi_node);

    plist_t vv_node = plist_new_uint(strtol(AIRPLAY_VV, NULL, 10));
    plist_dict_set_item(r_node, "vv", vv_node);

    plist_t status_flags_node = plist_new_uint(68);
    plist_dict_set_item(r_node, "statusFlags", status_flags_node);

    plist_t keep_alive_low_power_node = plist_new_uint(1);
    plist_dict_set_item(r_node, "keepAliveLowPower", keep_alive_low_power_node);

    plist_t source_version_node = plist_new_string(AIRPLAY_SRCVERS);
    plist_dict_set_item(r_node, "sourceVersion", source_version_node);

    plist_t pk_node = plist_new_data(pk, pk_len);
    plist_dict_set_item(r_node, "pk", pk_node);

    plist_t keep_alive_send_stats_as_body_node = plist_new_uint(1);
    plist_dict_set_item(r_node, "keepAliveSendStatsAsBody", keep_alive_send_stats_as_body_node);

    plist_t device_id_node = plist_new_string(hw_addr);
    plist_dict_set_item(r_node, "deviceID", device_id_node);

    plist_t audio_latencies_node = plist_new_array();
    plist_t audio_latencies_0_node = plist_new_dict();
    plist_t audio_latencies_0_output_latency_micros_node = plist_new_bool(0);
    plist_t audio_latencies_0_type_node = plist_new_uint(100);
    plist_t audio_latencies_0_audio_type_node = plist_new_string("default");
    plist_t audio_latencies_0_input_latency_micros_node = plist_new_bool(0);
    plist_dict_set_item(audio_latencies_0_node, "outputLatencyMicros", audio_latencies_0_output_latency_micros_node);
    plist_dict_set_item(audio_latencies_0_node, "type", audio_latencies_0_type_node);
    plist_dict_set_item(audio_latencies_0_node, "audioType", audio_latencies_0_audio_type_node);
    plist_dict_set_item(audio_latencies_0_node, "inputLatencyMicros", audio_latencies_0_input_latency_micros_node);
    plist_array_append_item(audio_latencies_node, audio_latencies_0_node);
    plist_t audio_latencies_1_node = plist_new_dict();
    plist_t audio_latencies_1_output_latency_micros_node = plist_new_bool(0);
    plist_t audio_latencies_1_type_node = plist_new_uint(101);
    plist_t audio_latencies_1_audio_type_node = plist_new_string("default");
    plist_t audio_latencies_1_input_latency_micros_node = plist_new_bool(0);
    plist_dict_set_item(audio_latencies_1_node, "outputLatencyMicros", audio_latencies_1_output_latency_micros_node);
    plist_dict_set_item(audio_latencies_1_node, "type", audio_latencies_1_type_node);
    plist_dict_set_item(audio_latencies_1_node, "audioType", audio_latencies_1_audio_type_node);
    plist_dict_set_item(audio_latencies_1_node, "inputLatencyMicros", audio_latencies_1_input_latency_micros_node);
    plist_array_append_item(audio_latencies_node, audio_latencies_1_node);
    plist_dict_set_item(r_node, "audioLatencies", audio_latencies_node);

    plist_t model_node = plist_new_string(GLOBAL_MODEL);
    plist_dict_set_item(r_node, "model", model_node);

    plist_t mac_address_node = plist_new_string(hw_addr);
    plist_dict_set_item(r_node, "macAddress", mac_address_node);

    plist_t displays_node = plist_new_array();
    plist_t displays_0_node = plist_new_dict();
    plist_t displays_0_uuid_node = plist_new_string("e0ff8a27-6738-3d56-8a16-cc53aacee925");
    plist_t displays_0_width_physical_node = plist_new_bool(0);
    plist_t displays_0_height_physical_node = plist_new_bool(0);
    plist_t displays_0_width_node = plist_new_uint(conn->raop->width);
    plist_t displays_0_height_node = plist_new_uint(conn->raop->height);
    plist_t displays_0_width_pixels_node = plist_new_uint(conn->raop->width);
    plist_t displays_0_height_pixels_node = plist_new_uint(conn->raop->height);
    plist_t displays_0_rotation_node = plist_new_bool(0);
    plist_t displays_0_refresh_rate_node = plist_new_uint(conn->raop->refreshRate);
    plist_t displays_0_max_fps_node = plist_new_uint(conn->raop->maxFPS);
    plist_t displays_0_overscanned_node = plist_new_bool(conn->raop->overscanned);
    plist_t displays_0_features = plist_new_uint(14);

    plist_dict_set_item(displays_0_node, "uuid", displays_0_uuid_node);
    plist_dict_set_item(displays_0_node, "widthPhysical", displays_0_width_physical_node);
    plist_dict_set_item(displays_0_node, "heightPhysical", displays_0_height_physical_node);
    plist_dict_set_item(displays_0_node, "width", displays_0_width_node);
    plist_dict_set_item(displays_0_node, "height", displays_0_height_node);
    plist_dict_set_item(displays_0_node, "widthPixels", displays_0_width_pixels_node);
    plist_dict_set_item(displays_0_node, "heightPixels", displays_0_height_pixels_node);
    plist_dict_set_item(displays_0_node, "rotation", displays_0_rotation_node);
    plist_dict_set_item(displays_0_node, "refreshRate", displays_0_refresh_rate_node);
    plist_dict_set_item(displays_0_node, "maxFPS", displays_0_max_fps_node);
    plist_dict_set_item(displays_0_node, "overscanned", displays_0_overscanned_node);
    plist_dict_set_item(displays_0_node, "features", displays_0_features);
    plist_array_append_item(displays_node, displays_0_node);
    plist_dict_set_item(r_node, "displays", displays_node);

    plist_to_bin(r_node, response_data, (uint32_t *) response_datalen);
    http_response_add_header(response, "Content-Type", "application/x-apple-binary-plist");
    free(pk);
    free(hw_addr);
}

static void
raop_handler_pairsetup(raop_conn_t *conn,
                       http_request_t *request, http_response_t *response,
                       char **response_data, int *response_datalen)
{
    unsigned char public_key[ED25519_KEY_SIZE];
    //const char *data;
    int datalen;

    //data =
    http_request_get_data(request, &datalen);
    if (datalen != 32) {
        logger_log(conn->raop->logger, LOGGER_ERR, "Invalid pair-setup data");
        return;
    }

    pairing_get_public_key(conn->raop->pairing, public_key);
    pairing_session_set_setup_status(conn->pairing);

    *response_data = malloc(sizeof(public_key));
    if (*response_data) {
        http_response_add_header(response, "Content-Type", "application/octet-stream");
        memcpy(*response_data, public_key, sizeof(public_key));
        *response_datalen = sizeof(public_key);
    }
}

static void
raop_handler_pairverify(raop_conn_t *conn,
                        http_request_t *request, http_response_t *response,
                        char **response_data, int *response_datalen)
{
    if (pairing_session_check_handshake_status(conn->pairing)) {
        return;
    }
    unsigned char public_key[X25519_KEY_SIZE];
    unsigned char signature[PAIRING_SIG_SIZE];
    const unsigned char *data;
    int datalen;

    data = (unsigned char *) http_request_get_data(request, &datalen);
    if (datalen < 4) {
        logger_log(conn->raop->logger, LOGGER_ERR, "Invalid pair-verify data");
        return;
    }
    switch (data[0]) {
        case 1:
            if (datalen != 4 + X25519_KEY_SIZE + X25519_KEY_SIZE) {
                logger_log(conn->raop->logger, LOGGER_ERR, "Invalid pair-verify data");
                return;
            }
            /* We can fall through these errors, the result will just be garbage... */
            if (pairing_session_handshake(conn->pairing, data + 4, data + 4 + X25519_KEY_SIZE)) {
                logger_log(conn->raop->logger, LOGGER_ERR, "Error initializing pair-verify handshake");
            }
            if (pairing_session_get_public_key(conn->pairing, public_key)) {
                logger_log(conn->raop->logger, LOGGER_ERR, "Error getting ECDH public key");
            }
            if (pairing_session_get_signature(conn->pairing, signature)) {
                logger_log(conn->raop->logger, LOGGER_ERR, "Error getting ED25519 signature");
            }
            *response_data = malloc(sizeof(public_key) + sizeof(signature));
            if (*response_data) {
                http_response_add_header(response, "Content-Type", "application/octet-stream");
                memcpy(*response_data, public_key, sizeof(public_key));
                memcpy(*response_data + sizeof(public_key), signature, sizeof(signature));
                *response_datalen = sizeof(public_key) + sizeof(signature);
            }
            break;
        case 0:
            logger_log(conn->raop->logger, LOGGER_DEBUG, "2nd pair-verify step: checking signature");
            if (datalen != 4 + PAIRING_SIG_SIZE) {
                logger_log(conn->raop->logger, LOGGER_ERR, "Invalid pair-verify data");
                return;
            }

            if (pairing_session_finish(conn->pairing, data + 4)) {
                logger_log(conn->raop->logger, LOGGER_ERR, "Incorrect pair-verify signature");
                http_response_set_disconnect(response, 1);
                return;
            }
            logger_log(conn->raop->logger, LOGGER_DEBUG, "pair-verify: signature is verified");	    
            http_response_add_header(response, "Content-Type", "application/octet-stream");
            break;
    }
}

static void
raop_handler_fpsetup(raop_conn_t *conn,
                     http_request_t *request, http_response_t *response,
                     char **response_data, int *response_datalen)
{
    const unsigned char *data;
    int datalen;

    data = (unsigned char *) http_request_get_data(request, &datalen);
    if (datalen == 16) {
        *response_data = malloc(142);
        if (*response_data) {
            http_response_add_header(response, "Content-Type", "application/octet-stream");
            if (!fairplay_setup(conn->fairplay, data, (unsigned char *) *response_data)) {
                *response_datalen = 142;
            } else {
                // Handle error?
                free(*response_data);
                *response_data = NULL;
            }
        }
    } else if (datalen == 164) {
        *response_data = malloc(32);
        if (*response_data) {
            http_response_add_header(response, "Content-Type", "application/octet-stream");
            if (!fairplay_handshake(conn->fairplay, data, (unsigned char *) *response_data)) {
                *response_datalen = 32;
            } else {
                // Handle error?
                free(*response_data);
                *response_data = NULL;
            }
        }
    } else {
        logger_log(conn->raop->logger, LOGGER_ERR, "Invalid fp-setup data length");
        return;
    }
}

static void
raop_handler_options(raop_conn_t *conn,
                     http_request_t *request, http_response_t *response,
                     char **response_data, int *response_datalen)
{
    http_response_add_header(response, "Public", "SETUP, RECORD, PAUSE, FLUSH, TEARDOWN, OPTIONS, GET_PARAMETER, SET_PARAMETER");
}

static void
raop_handler_setup(raop_conn_t *conn,
                   http_request_t *request, http_response_t *response,
                   char **response_data, int *response_datalen)
{
    unsigned short remote_cport = 0;

    const char *transport;
    int use_udp;
    const char *dacp_id;
    const char *active_remote_header;

    const char *data;
    int data_len;

    data = http_request_get_data(request, &data_len);

    dacp_id = http_request_get_header(request, "DACP-ID");
    active_remote_header = http_request_get_header(request, "Active-Remote");

    if (dacp_id && active_remote_header) {
        logger_log(conn->raop->logger, LOGGER_DEBUG, "DACP-ID: %s", dacp_id);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "Active-Remote: %s", active_remote_header);
        if (conn->raop_rtp) {
            raop_rtp_remote_control_id(conn->raop_rtp, dacp_id, active_remote_header);
        }
    }

    transport = http_request_get_header(request, "Transport");
    if (transport) {
        logger_log(conn->raop->logger, LOGGER_DEBUG, "Transport: %s", transport);
        use_udp = strncmp(transport, "RTP/AVP/TCP", 11);
    } else {
        logger_log(conn->raop->logger, LOGGER_DEBUG, "Transport: null");
        use_udp = 0;
    }

    // Parsing bplist
    plist_t req_root_node = NULL;
    plist_from_bin(data, data_len, &req_root_node);
    plist_t req_ekey_node = plist_dict_get_item(req_root_node, "ekey");
    plist_t req_eiv_node = plist_dict_get_item(req_root_node, "eiv");
	
    // For the response
    plist_t res_root_node = plist_new_dict();

    if (PLIST_IS_DATA(req_eiv_node) && PLIST_IS_DATA(req_ekey_node)) {
        // The first SETUP call that initializes keys and timing

        unsigned char aesiv[16];
        unsigned char aeskey_old[16];
        unsigned char aeskey[16];
        unsigned char *aeskey_audio = aeskey;
        unsigned char *aeskey_video = aeskey;

	
        logger_log(conn->raop->logger, LOGGER_DEBUG, "SETUP 1");

        // First setup
        char* eiv = NULL;
        uint64_t eiv_len = 0;
        plist_get_data_val(req_eiv_node, &eiv, &eiv_len);
        memcpy(aesiv, eiv, 16);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "eiv_len = %llu", eiv_len);
        char* str = utils_data_to_string(aesiv, 16, 16);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "16 byte aesiv (needed for AES-CBC audio decryption iv):\n%s", str);
        free(str);

        char* ekey = NULL;
        uint64_t ekey_len = 0;
        plist_get_data_val(req_ekey_node, &ekey, &ekey_len);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "ekey_len = %llu", ekey_len);
        // ekey is 72 bytes, aeskey is 16 bytes
        str = utils_data_to_string((unsigned char *) ekey, ekey_len, 16);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "ekey:\n%s", str);
        free (str);

        int ret = fairplay_decrypt(conn->fairplay, (unsigned char*) ekey, aeskey_old);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "fairplay_decrypt ret = %d", ret);
        str = utils_data_to_string(aeskey_old, 16, 16);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "16 byte aeskey (fairplay-decrypted from ekey):\n%s", str);
        free(str);

        unsigned char ecdh_secret[X25519_KEY_SIZE];
        pairing_get_ecdh_secret_key(conn->pairing, ecdh_secret);
        str = utils_data_to_string(ecdh_secret, X25519_KEY_SIZE, 16);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "32 byte shared ecdh_secret:\n%s", str);
        free(str);

        unsigned char eaeskey[64] = {};
        memcpy(eaeskey, aeskey_old, 16);
        sha_ctx_t *ctx = sha_init();
        sha_update(ctx, eaeskey, 16);
        sha_update(ctx, ecdh_secret, 32);
        sha_final(ctx, eaeskey, NULL);
        sha_destroy(ctx);
        memcpy(aeskey, eaeskey, 16);

        str = utils_data_to_string(aeskey, 16, 16);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "16 byte aeskey after sha-256 hash with ecdh_secret:\n%s", str);
        free(str);
	
        /* old-protocol clients such as AirMyPC use the unhashed key aeskey_old for both audio and video   (ios9, ios10 support)*/
        /* clients with sourceVersion <= OLD_PROTOCOL_AUDIO_CLIENT_SOURCEVERSION use unhashed aeskey_old for audio decryption */
        /* this is also done for audio or video if clients User-Agent string is included in OLD_PROTOCOL_[AUDIO,VIDEO]_CLIENT_LIST  (AirMyPC support)*/
        /* OLD_PROTOCOL_AUDIO_CLIENT_LIST, OLD_PROTOCOL_VIDEO_CLIENT_LIST, OLD_PROTOCOL_AUDIO_CLIENT_SOURCEVERSION are defined in global.h */

        plist_t req_source_version_node = plist_dict_get_item(req_root_node, "sourceVersion");
        char *sourceVersion;
        plist_get_string_val(req_source_version_node, &sourceVersion);
        const char *user_agent = http_request_get_header(request, "User-Agent");
        logger_log(conn->raop->logger, LOGGER_INFO, "Client identified as User-Agent: %s, sourceVersion: %s", user_agent, sourceVersion);	

#ifdef OLD_PROTOCOL_AUDIO_CLIENT_SOURCEVERSION
        char *end_ptr;
        if (strtoul(sourceVersion, &end_ptr, 10) <= strtoul(OLD_PROTOCOL_AUDIO_CLIENT_SOURCEVERSION , &end_ptr, 10)) aeskey_audio = aeskey_old;
#endif

#ifdef OLD_PROTOCOL_AUDIO_CLIENT_USER_AGENT_LIST
        if (strstr(OLD_PROTOCOL_AUDIO_CLIENT_USER_AGENT_LIST, user_agent)) aeskey_audio = aeskey_old;
#endif

#ifdef OLD_PROTOCOL_VIDEO_CLIENT_USER_AGENT_LIST
        if (strstr(OLD_PROTOCOL_AUDIO_CLIENT_USER_AGENT_LIST, user_agent)) aeskey_video = aeskey_old;
#endif

        if (aeskey_audio == aeskey_old) {
            logger_log(conn->raop->logger, LOGGER_INFO, "Client identifed as using old protocol (unhashed) AES audio key)");
        }

        if (aeskey_video == aeskey_old) {
            logger_log(conn->raop->logger, LOGGER_INFO, "Client identifed as using old protocol (unhashed) AES video key)");
        }

        // Time port
        uint64_t timing_rport;
        plist_t time_note = plist_dict_get_item(req_root_node, "timingPort");
        plist_get_uint_val(time_note, &timing_rport);
        logger_log(conn->raop->logger, LOGGER_DEBUG, "timing_rport = %llu", timing_rport);

        unsigned short timing_lport = conn->raop->timing_lport;
        conn->raop_ntp = raop_ntp_init(conn->raop->logger, conn->remote, conn->remotelen, timing_rport);
        raop_ntp_start(conn->raop_ntp, &timing_lport);

        conn->raop_rtp = raop_rtp_init(conn->raop->logger, &conn->raop->callbacks, conn->raop_ntp, conn->remote, conn->remotelen, aeskey_audio, aesiv);
        conn->raop_rtp_mirror = raop_rtp_mirror_init(conn->raop->logger, &conn->raop->callbacks, conn->raop_ntp, conn->remote, conn->remotelen, aeskey_video);

        plist_t res_event_port_node = plist_new_uint(conn->raop->port);
        plist_t res_timing_port_node = plist_new_uint(timing_lport);
        plist_dict_set_item(res_root_node, "timingPort", res_timing_port_node);
        plist_dict_set_item(res_root_node, "eventPort", res_event_port_node);

        logger_log(conn->raop->logger, LOGGER_DEBUG, "eport = %d, tport = %d", conn->raop->port, timing_lport);
    }

    // Process stream setup requests
    plist_t req_streams_node = plist_dict_get_item(req_root_node, "streams");
    if (PLIST_IS_ARRAY(req_streams_node)) {
        plist_t res_streams_node = plist_new_array();

        int count = plist_array_get_size(req_streams_node);
        for (int i = 0; i < count; i++) {
            plist_t req_stream_node = plist_array_get_item(req_streams_node, i);
            plist_t req_stream_type_node = plist_dict_get_item(req_stream_node, "type");
            uint64_t type;
            plist_get_uint_val(req_stream_type_node, &type);
            logger_log(conn->raop->logger, LOGGER_DEBUG, "type = %llu", type);

            switch (type) {
                case 110: {
                    // Mirroring
                    unsigned short dport = conn->raop->mirror_data_lport;
                    plist_t stream_id_node = plist_dict_get_item(req_stream_node, "streamConnectionID");
                    uint64_t stream_connection_id;
                    plist_get_uint_val(stream_id_node, &stream_connection_id);
                    logger_log(conn->raop->logger, LOGGER_DEBUG, "streamConnectionID (needed for AES-CTR video decryption iv): %llu", stream_connection_id);

                    if (conn->raop_rtp_mirror) {
                        raop_rtp_init_mirror_aes(conn->raop_rtp_mirror, stream_connection_id);
                        raop_rtp_start_mirror(conn->raop_rtp_mirror, use_udp, &dport);
                        logger_log(conn->raop->logger, LOGGER_DEBUG, "Mirroring initialized successfully");
                    } else {
                        logger_log(conn->raop->logger, LOGGER_ERR, "Mirroring not initialized at SETUP, playing will fail!");
                        http_response_set_disconnect(response, 1);
                    }

                    plist_t res_stream_node = plist_new_dict();
                    plist_t res_stream_data_port_node = plist_new_uint(dport);
                    plist_t res_stream_type_node = plist_new_uint(110);
                    plist_dict_set_item(res_stream_node, "dataPort", res_stream_data_port_node);
                    plist_dict_set_item(res_stream_node, "type", res_stream_type_node);
                    plist_array_append_item(res_streams_node, res_stream_node);

                    break;
                } case 96: {
                    // Audio
                    unsigned short cport = conn->raop->control_lport, dport = conn->raop->data_lport; 

                    uint64_t audioFormat;
		    unsigned char ct;
	            unsigned short spf;
                    bool isMedia; 
                    bool usingScreen;

                    if (conn->raop->callbacks.audio_get_format) {
		        /* get audio compression type */
		        uint64_t uint_val;
                        uint8_t bool_val;
			
                        plist_t req_stream_ct_node = plist_dict_get_item(req_stream_node, "ct");
                        plist_get_uint_val(req_stream_ct_node, &uint_val);
                        ct = (unsigned char) uint_val;

                        plist_t req_stream_spf_node = plist_dict_get_item(req_stream_node, "spf");
                        plist_get_uint_val(req_stream_spf_node, &uint_val);
                        spf = (unsigned short) uint_val;
		    
                        plist_t req_stream_audio_format_node = plist_dict_get_item(req_stream_node, "audioFormat");
                        plist_get_uint_val(req_stream_audio_format_node, &audioFormat);
			
                        plist_t req_stream_ismedia_node = plist_dict_get_item(req_stream_node, "isMedia");
		        if (req_stream_ismedia_node) {
		            plist_get_bool_val(req_stream_ismedia_node, &bool_val);
			    isMedia = (bool) bool_val;
                        } else {
                            isMedia = false;
                        }

                        plist_t req_stream_usingscreen_node = plist_dict_get_item(req_stream_node, "usingScreen");
                        if (req_stream_usingscreen_node) {
                            plist_get_bool_val(req_stream_usingscreen_node, &bool_val);
                            usingScreen = (bool) bool_val;
                        } else {
                            usingScreen = false;
                        }

                        conn->raop->callbacks.audio_get_format(conn->raop->callbacks.cls, &ct, &spf, &usingScreen, &isMedia, &audioFormat);
                    }

                    if (conn->raop_rtp) {
                        raop_rtp_start_audio(conn->raop_rtp, use_udp, remote_cport, &cport, &dport);
                        logger_log(conn->raop->logger, LOGGER_DEBUG, "RAOP initialized success");
                    } else {
                        logger_log(conn->raop->logger, LOGGER_ERR, "RAOP not initialized at SETUP, playing will fail!");
                        http_response_set_disconnect(response, 1);
                    }

                    plist_t res_stream_node = plist_new_dict();
                    plist_t res_stream_data_port_node = plist_new_uint(dport);
                    plist_t res_stream_control_port_node = plist_new_uint(cport);
                    plist_t res_stream_type_node = plist_new_uint(96);
                    plist_dict_set_item(res_stream_node, "dataPort", res_stream_data_port_node);
                    plist_dict_set_item(res_stream_node, "controlPort", res_stream_control_port_node);
                    plist_dict_set_item(res_stream_node, "type", res_stream_type_node);
                    plist_array_append_item(res_streams_node, res_stream_node);

                    break;
                }

                default:
                    logger_log(conn->raop->logger, LOGGER_ERR, "SETUP tries to setup stream of unknown type %llu", type);
                    http_response_set_disconnect(response, 1);
                    break;
            }
        }

        plist_dict_set_item(res_root_node, "streams", res_streams_node);
    }

    plist_to_bin(res_root_node, response_data, (uint32_t*) response_datalen);
    http_response_add_header(response, "Content-Type", "application/x-apple-binary-plist");
}

static void
raop_handler_get_parameter(raop_conn_t *conn,
                           http_request_t *request, http_response_t *response,
                           char **response_data, int *response_datalen)
{
    const char *content_type;
    const char *data;
    int datalen;

    content_type = http_request_get_header(request, "Content-Type");
    data = http_request_get_data(request, &datalen);
    if (!strcmp(content_type, "text/parameters")) {
        const char *current = data;

        while (current && (datalen - (current - data) > 0)) {
            const char *next;

            /* This is a bit ugly, but seems to be how airport works too */
            if ((datalen - (current - data) >= 8) && !strncmp(current, "volume\r\n", 8)) {
                const char volume[] = "volume: 0.0\r\n";

                http_response_add_header(response, "Content-Type", "text/parameters");
                *response_data = strdup(volume);
                if (*response_data) {
                    *response_datalen = strlen(*response_data);
                }
                return;
            }

            for (next = current ; (datalen - (next - data) > 0) ; ++next)
                if (*next == '\r')
                    break;

            if ((datalen - (next - data) >= 2) && !strncmp(next, "\r\n", 2)) {
                if ((next - current) > 0) {
                    logger_log(conn->raop->logger, LOGGER_WARNING,
                               "Found an unknown parameter: %.*s", (next - current), current);
                }
                current = next + 2;
            } else {
                current = NULL;
            }
        }
    }
}

static void
raop_handler_set_parameter(raop_conn_t *conn,
                           http_request_t *request, http_response_t *response,
                           char **response_data, int *response_datalen)
{
    const char *content_type;
    const char *data;
    int datalen;

    content_type = http_request_get_header(request, "Content-Type");
    data = http_request_get_data(request, &datalen);
    if (!strcmp(content_type, "text/parameters")) {
        char *datastr;
        datastr = calloc(1, datalen+1);
        if (data && datastr && conn->raop_rtp) {
            memcpy(datastr, data, datalen);
            if ((datalen >= 8) && !strncmp(datastr, "volume: ", 8)) {
                float vol = 0.0;
                sscanf(datastr+8, "%f", &vol);
                raop_rtp_set_volume(conn->raop_rtp, vol);
            } else if ((datalen >= 10) && !strncmp(datastr, "progress: ", 10)) {
                unsigned int start, curr, end;
                sscanf(datastr+10, "%u/%u/%u", &start, &curr, &end);
                raop_rtp_set_progress(conn->raop_rtp, start, curr, end);
            }
        } else if (!conn->raop_rtp) {
            logger_log(conn->raop->logger, LOGGER_WARNING, "RAOP not initialized at SET_PARAMETER");
        }
        free(datastr);
    } else if (!strcmp(content_type, "image/jpeg") || !strcmp(content_type, "image/png")) {
        logger_log(conn->raop->logger, LOGGER_DEBUG, "Got image data of %d bytes", datalen);
        if (conn->raop_rtp) {
            raop_rtp_set_coverart(conn->raop_rtp, data, datalen);
        } else {
            logger_log(conn->raop->logger, LOGGER_WARNING, "RAOP not initialized at SET_PARAMETER coverart");
        }
    } else if (!strcmp(content_type, "application/x-dmap-tagged")) {
        logger_log(conn->raop->logger, LOGGER_DEBUG, "Got metadata of %d bytes", datalen);
        if (conn->raop_rtp) {
            raop_rtp_set_metadata(conn->raop_rtp, data, datalen);
        } else {
            logger_log(conn->raop->logger, LOGGER_WARNING, "RAOP not initialized at SET_PARAMETER metadata");
        }
    }
}


static void
raop_handler_feedback(raop_conn_t *conn,
                      http_request_t *request, http_response_t *response,
                      char **response_data, int *response_datalen)
{
    logger_log(conn->raop->logger, LOGGER_DEBUG, "raop_handler_feedback");
}

static void
raop_handler_record(raop_conn_t *conn,
                    http_request_t *request, http_response_t *response,
                    char **response_data, int *response_datalen)
{
    logger_log(conn->raop->logger, LOGGER_DEBUG, "raop_handler_record");
    http_response_add_header(response, "Audio-Latency", "11025");
    http_response_add_header(response, "Audio-Jack-Status", "connected; type=analog");
}
