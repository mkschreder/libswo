/*
 * This file is part of the libswo project.
 *
 * Copyright (C) 2014-2015 Marc Schink <swo-dev@marcschink.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBSWO_LIBSWO_H
#define LIBSWO_LIBSWO_H

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 *
 * Public libswo header file to be used by applications.
 */

/** Error codes returned by libswo functions. */
enum libswo_error {
	/** No error. */
	LIBSWO_OK = 0,
	/** Unspecific error. */
	LIBSWO_ERR = -1,
	/** Memory allocation error. */
	LIBSWO_ERR_MALLOC = -2,
	/** Invalid argument. */
	LIBSWO_ERR_ARG = -3
};

/** libswo log levels. */
enum libswo_log_level {
	/** Output no messages. */
	LIBSWO_LOG_LEVEL_NONE = 0,
	/** Output error messages. */
	LIBSWO_LOG_LEVEL_ERROR = 1,
	/** Output warnings. */
	LIBSWO_LOG_LEVEL_WARNING = 2,
	/** Output informational messages. */
	LIBSWO_LOG_LEVEL_INFO = 3,
	/** Output debug messages. */
	LIBSWO_LOG_LEVEL_DEBUG = 4
};


/** Default libswo log domain. */
#define LIBSWO_LOG_DOMAIN_DEFAULT	"libswo: "

/** Maximum length of a libswo log domain in bytes. */
#define LIBSWO_LOG_DOMAIN_MAX_LENGTH	32

/** Packet types. */
enum libswo_packet_type {
	/** Unknown data packet. */
	LIBSWO_PACKET_TYPE_UNKNOWN = 0,
	/** Synchronization packet. */
	LIBSWO_PACKET_TYPE_SYNC = 1,
	/** Overflow packet. */
	LIBSWO_PACKET_TYPE_OVERFLOW = 2,
	/** Local timestamp packet. */
	LIBSWO_PACKET_TYPE_LTS = 3,
	/** Global timestamp (GTS1) packet. */
	LIBSWO_PACKET_TYPE_GTS1 = 4,
	/** Global timestamp (GTS2) packet. */
	LIBSWO_PACKET_TYPE_GTS2 = 5,
	/** Extension packet. */
	LIBSWO_PACKET_TYPE_EXT = 6,
	/** Instrumentation packet. */
	LIBSWO_PACKET_TYPE_INST = 7,
	/** Hardware source packet. */
	LIBSWO_PACKET_TYPE_HW = 8,
	/** DWT: Event counter packet. */
	LIBSWO_PACKET_TYPE_DWT_EVTCNT = 16,
	/** DWT: Exception trace packet. */
	LIBSWO_PACKET_TYPE_DWT_EXCTRACE = 17,
	/** DWT: Periodic PC sample packet. */
	LIBSWO_PACKET_TYPE_DWT_PC_SAMPLE = 18,
	/** DWT: Data trace PC value packet. */
	LIBSWO_PACKET_TYPE_DWT_PC_VALUE = 19,
	/** DWT: Data trace address offset packet. */
	LIBSWO_PACKET_TYPE_DWT_ADDR_OFFSET = 20,
	/** DWT: Data trace data value packet. */
	LIBSWO_PACKET_TYPE_DWT_DATA_VALUE = 21
};

/** Local timestamp relation information. */
enum libswo_lts_relation {
	/** Source and timestamp packet are synchronous. */
	LIBSWO_LTS_REL_SYNC = 0,
	/** Timestamp packet is delayed relative to the source packet. */
	LIBSWO_LTS_REL_TS = 1,
	/** Source packet is delayed relative to the local timestamp packet. */
	LIBSWO_LTS_REL_SRC = 2,
	/** Source and timestamp packet are both delayed. */
	LIBSWO_LTS_REL_BOTH = 3
};

/** Extension sources. */
enum libswo_ext_source {
	/** Instrumentation Trace Macrocell (ITM) extension packet. */
	LIBSWO_EXT_SRC_ITM = 0,
	/** Hardware source extension packet. */
	LIBSWO_EXT_SRC_HW = 1
};

/** Decoder flags. */
enum libswo_decoder_flags {
	/**
	 * Indicates the end of the data stream to the decoder.
	 *
	 * If this flag is set, the decoder treats all remaining and incomplete
	 * packets as unknown data.
	 */
	LIBSWO_DF_EOS = (1 << 0)
};

/** Exception trace functions. */
enum libswo_exctrace_function {
	/** Reserved. */
	LIBSWO_EXCTRACE_FUNC_RESERVED = 0,
	/** Enter exception. */
	LIBSWO_EXCTRACE_FUNC_ENTER = 1,
	/** Exit exception. */
	LIBSWO_EXCTRACE_FUNC_EXIT = 2,
	/** Return to exception. */
	LIBSWO_EXCTRACE_FUNC_RETURN = 3
};

/** Maximum payload size of a packet in bytes. */
#define LIBSWO_MAX_PAYLOAD_SIZE		4

/** Maximum address of a source packet. */
#define LIBSWO_MAX_SOURCE_ADDRESS	32

/**
 * Common fields packet.
 *
 * Helper which contains the fields that are common to all packets.
 */
struct libswo_packet_any {
	/** Packet type. */
	enum libswo_packet_type type;
	/**
	 * Packet size.
	 *
	 * Interpretation of this value depends on the specific packet type.
	 */
	size_t size;
	/**
	 * Packet data.
	 *
	 * Interpretation of this field depends on the specific packet type.
	 */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
};

/**
 * Unknown data packet.
 *
 * This packet represents data which could not be decoded.
 */
struct libswo_packet_unknown {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
};

/** Synchronization packet. */
struct libswo_packet_sync {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size in bits. */
	size_t size;
	/**
	 * Packet data.
	 *
	 * @note This field contains no valid data and must not be used.
	 */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
};

/** Overflow packet. */
struct libswo_packet_of {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
};

/** Local timestamp packet. */
struct libswo_packet_lts {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/**
	 * Indicates the relationship between the generation of local timestamp
	 * packet and the corresponding source packet.
	 */
	enum libswo_lts_relation relation;
	/** Local timestamp value. */
	uint32_t value;
};

/** Global timestamp (GTS1) packet. */
struct libswo_packet_gts1 {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Low-order bits [25:0] of the global timestamp value. */
	uint32_t value;
	/**
	 * Indicates if the system has asserted the clock change input to the
	 * processor since the last emitted global timestamp packet.
	 */
	bool clkch;
	/**
	 * Indicates if the high-order global timestamp bits [47:26] changed
	 * since the last emitted global timestamp (GTS2) packet.
	 */
	bool wrap;
};

/** Global timestamp (GTS2) packet. */
struct libswo_packet_gts2 {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** High-order bits [47:26] of the global timestamp value. */
	uint32_t value;
};

/** Extension packet. */
struct libswo_packet_ext {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Source of the extension packet. */
	enum libswo_ext_source source;
	/** Extension information. */
	uint32_t value;
};

/** Instrumentation packet. */
struct libswo_packet_inst {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Port number. */
	uint8_t address;
	/** Instrumentation data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
};

/** Hardware source packet. */
struct libswo_packet_hw {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Address. */
	uint8_t address;
	/** Hardware source data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
};

/** DWT: Event counter packet. */
struct libswo_packet_dwt_evtcnt {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Address. */
	uint8_t address;
	/** Hardware source data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
	/** Indicates whether the CPICNT value wrapped around to zero. */
	bool cpi;
	/** Indicates whether the EXCCNT value wrapped around to zero. */
	bool exc;
	/** Indicates whether the SLEEPCNT value wrapped around to zero. */
	bool sleep;
	/** Indicates whether the LSUCNT value wrapped around to zero. */
	bool lsu;
	/** Indicates whether the FOLDCNT value wrapped around to zero. */
	bool fold;
	/** Indicates whether the CYCCNT value wrapped around to zero. */
	bool cyc;
};

/** DWT: Exception trace packet. */
struct libswo_packet_dwt_exctrace {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Address. */
	uint8_t address;
	/** Hardware source data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
	/** Exception number. */
	uint16_t exception;
	/** Action taken by the processor. */
	enum libswo_exctrace_function function;
};

/** DWT: Periodic PC sample packet. */
struct libswo_packet_dwt_pc_sample {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Address. */
	uint8_t address;
	/** Hardware source data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
	/** Indicates whether the processor is in sleep mode. */
	bool sleep;
	/** Program counter (PC) value. */
	uint32_t pc;
};

/** DWT: Data trace PC value packet. */
struct libswo_packet_dwt_pc_value {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Address. */
	uint8_t address;
	/** Hardware source data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
	/** Number of the comparator that generated the packet. */
	uint8_t cmpn;
	/** Program counter (PC) value. */
	uint32_t pc;
};

/** DWT: Data trace address offset packet. */
struct libswo_packet_dwt_addr_offset {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Address. */
	uint8_t address;
	/** Hardware source data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
	/** Number of the comparator that generated the packet. */
	uint8_t cmpn;
	/** Address offset. */
	uint16_t offset;
};

/** DWT: Data trace data value packet. */
struct libswo_packet_dwt_data_value {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Packet size including the header in bytes. */
	size_t size;
	/** Packet data. */
	uint8_t data[1 + LIBSWO_MAX_PAYLOAD_SIZE];
	/** Address. */
	uint8_t address;
	/** Hardware source data. */
	uint8_t payload[LIBSWO_MAX_PAYLOAD_SIZE];
	/** Integer representation of the payload. */
	uint32_t value;
	/** Indicates whether it was a write or read access. */
	bool wnr;
	/** Number of the comparator that generated the packet. */
	uint8_t cmpn;
	/** Data value. */
	uint32_t data_value;
};

/** Union of all packet types. */
union libswo_packet {
	/** Packet type. */
	enum libswo_packet_type type;
	/** Common fields packet. */
	struct libswo_packet_any any;
	/** Unknown data packet. */
	struct libswo_packet_unknown unknown;
	/** Synchronization packet. */
	struct libswo_packet_sync sync;
	/** Overflow packet. */
	struct libswo_packet_of of;
	/** Local timestamp packet. */
	struct libswo_packet_lts lts;
	/** Global timestamp (GTS1) packet. */
	struct libswo_packet_gts1 gts1;
	/** Global timestamp (GTS2) packet. */
	struct libswo_packet_gts2 gts2;
	/** Extension packet. */
	struct libswo_packet_ext ext;
	/** Instrumentation packet. */
	struct libswo_packet_inst inst;
	/** Hardware source packet. */
	struct libswo_packet_hw hw;
	/** DWT: Event counter packet. */
	struct libswo_packet_dwt_evtcnt evtcnt;
	/** DWT: Exception trace packet. */
	struct libswo_packet_dwt_exctrace exctrace;
	/** DWT: Periodic PC sample packet. */
	struct libswo_packet_dwt_pc_sample pc_sample;
	/** DWT: Data trace PC value packet. */
	struct libswo_packet_dwt_pc_value pc_value;
	/** DWT: Data trace address offset packet. */
	struct libswo_packet_dwt_addr_offset addr_offset;
	/** DWT: Data trace data value packet. */
	struct libswo_packet_dwt_data_value data_value;
};

/**
 * @struct libswo_context
 *
 * Opaque structure representing a libswo context.
 */
struct libswo_context;

/**
 * Decoder callback function type.
 *
 * @param[in,out] ctx libswo context.
 * @param[out] packet Decoded packet.
 * @param[in,out] user_data User data passed to the callback function.
 *
 * @retval true Continue decoding.
 * @retval false Stop decoding.
 */
typedef int (*libswo_decoder_callback)(struct libswo_context *ctx,
		const union libswo_packet *packet, void *user_data);

/**
 * Log callback function type.
 *
 * @param[in,out] ctx libswo context.
 * @param[in] level Log level.
 * @param[in] format Message format in printf()-style.
 * @param[in] args Message arguments.
 * @param[in,out] user_data User data passed to the callback function.
 *
 * @return Number of characters printed on success, or a negative error code on
 *         failure.
 */
typedef int (*libswo_log_callback)(struct libswo_context *ctx,
		enum libswo_log_level level, const char *format,
		va_list args, void *user_data);

/** Macro to mark private libswo symbol. */
#if defined(_WIN32) || defined(__MSYS__) || defined(__CYGWIN__)
#define LIBSWO_PRIV
#else
#define LIBSWO_PRIV __attribute__ ((visibility ("hidden")))
#endif

/** Macro to mark public libswo API symbol. */
#ifdef _WIN32
#define LIBSWO_API
#else
#define LIBSWO_API __attribute__ ((visibility ("default")))
#endif

/*--- core.c ----------------------------------------------------------------*/

LIBSWO_API int libswo_init(struct libswo_context **ctx, uint8_t *buffer,
		size_t buffer_size);
LIBSWO_API int libswo_exit(struct libswo_context *ctx);

/*--- decoder.c -------------------------------------------------------------*/

LIBSWO_API int libswo_feed(struct libswo_context *ctx, const uint8_t *buffer,
		size_t length);
LIBSWO_API int libswo_decode(struct libswo_context *ctx, uint32_t flags);
LIBSWO_API int libswo_set_callback(struct libswo_context *ctx,
		libswo_decoder_callback callback, void *user_data);

/*--- error.c ---------------------------------------------------------------*/

LIBSWO_API const char *libswo_strerror(int error_code);
LIBSWO_API const char *libswo_strerror_name(int error_code);

/*--- log.c -----------------------------------------------------------------*/

LIBSWO_API int libswo_log_set_level(struct libswo_context *ctx,
		enum libswo_log_level level);
LIBSWO_API int libswo_log_get_level(const struct libswo_context *ctx,
		enum libswo_log_level *level);
LIBSWO_API int libswo_log_set_callback(struct libswo_context *ctx,
		libswo_log_callback callback, void *user_data);
LIBSWO_API int libswo_log_set_domain(struct libswo_context *ctx,
		const char *domain);
LIBSWO_API const char *libswo_log_get_domain(const struct libswo_context *ctx);

/*--- version.c -------------------------------------------------------------*/

LIBSWO_API int libswo_version_package_get_major(void);
LIBSWO_API int libswo_version_package_get_minor(void);
LIBSWO_API int libswo_version_package_get_micro(void);
LIBSWO_API const char *libswo_version_package_get_string(void);
LIBSWO_API int libswo_version_library_get_current(void);
LIBSWO_API int libswo_version_library_get_revision(void);
LIBSWO_API int libswo_version_library_get_age(void);
LIBSWO_API const char *libswo_version_library_get_string(void);

#include "version.h"

#ifdef __cplusplus
}
#endif

#endif /* LIBSWO_LIBSWO_H */
