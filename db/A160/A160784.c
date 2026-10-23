/*
 * A160784  Numbers that have a "v" in the middle of their names in Spanish.
 * https://oeis.org/A160784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having a "v" in the middle of their Spanish names.
   The implementation tests every integer in order, builds the
   Spanish name (without spaces) and checks whether the central
   character is 'v'.  The result is memoised so that repeated calls
   are fast.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"

static const char *const UNITS[10] = {
    "cero","uno","dos","tres","cuatro","cinco",
    "seis","siete","ocho","nueve"
};

static const char *const TEENS[10] = {
    "diez","once","doce","trece","catorce","quince",
    "dieciseis","diecisiete","dieciocho","diecinueve"
};

static const char *const TENS[10] = {
    "","","veinte","treinta","cuarenta","cincuenta",
    "sesenta","setenta","ochenta","noventa"
};

static const char *const HUNDREDS[9] = {
    "ciento","doscientos","trescientos","cuatrocientos",
    "quinientos","seiscientos","setecientos","ochocientos","novecientos"
};

static const char *const VEINTI[10] = {
    "", "veintiuno","veintidos","veintitres","veinticuatro",
    "veinticinco","veintiseis","veintisiete","veintiocho","veintinueve"
};

/* Append a C‑string to a buffer. */
static inline void append_str(const char *s, size_t *len, char *buf)
{
    while (*s) {
        buf[(*len)++] = *s++;
    }
}

/* Build words for a number less than 100. */
static void build_lt100(idx_t n, char *buf, size_t *len)
{
    if (n < 10) {
        append_str(UNITS[n], len, buf);
        return;
    }
    if (n < 20) {
        append_str(TEENS[n-10], len, buf);
        return;
    }
    idx_t tens = n / 10;
    idx_t unit = n % 10;
    if (tens == 2) {               /* 20‑29 */
        if (unit == 0) {
            append_str("veinte", len, buf);
            return;
        }
        append_str(VEINTI[unit], len, buf);
        return;
    }
    append_str(TENS[tens], len, buf);
    if (unit) {
        append_str("y", len, buf);
        append_str(UNITS[unit], len, buf);
    }
}

/* Build words for a number less than 1000. */
static void build_lt1000(idx_t n, char *buf, size_t *len)
{
    if (n == 0) return;

    if (n >= 100) {
        if (n == 100) {
            append_str("cien", len, buf);
            return;
        }
        idx_t h = n / 100;
        idx_t rem = n % 100;
        if (h == 1)
            append_str("ciento", len, buf);
        else
            append_str(HUNDREDS[h-1], len, buf);
        if (rem) build_lt100(rem, buf, len);
        return;
    }
    build_lt100(n, buf, len);
}

/* Build the full Spanish name for any positive integer. */
static void build_name(idx_t n, char *buf, size_t *len)
{
    *len = 0;
    if (n >= 1000000) {
        idx_t millions = n / 1000000;
        idx_t rem = n % 1000000;
        if (millions == 1) {
            append_str("millon", len, buf);
        } else {
            /* millions can be up to 999 for n < 1 000 000 000 */
            build_lt1000(millions, buf, len);
            append_str("millones", len, buf);
        }
        if (rem) {
            if (rem >= 1000) {
                /* thousands part of the remainder */
                idx_t thousands = rem / 1000;
                idx_t r = rem % 1000;
                build_lt1000(thousands, buf, len);
                append_str("mil", len, buf);
                if (r) build_lt1000(r, buf, len);
            } else {
                build_lt1000(rem, buf, len);
            }
        }
    } else if (n >= 1000) {
        idx_t thousands = n / 1000;
        idx_t rem = n % 1000;
        build_lt1000(thousands, buf, len);
        append_str("mil", len, buf);
        if (rem) build_lt1000(rem, buf, len);
    } else {
        build_lt1000(n, buf, len);
    }
}

/* Does the number have a 'v' as the central character of its Spanish name? */
static bool is_center_v(idx_t n)
{
    char buf[256];
    size_t len;
    build_name(n, buf, &len);
    if (len == 0 || (len & 1) == 0) return false;            /* even length */
    return buf[len/2] == 'v';
}

/* Memoised cache for the sequence. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

/* Ensure the cache contains at least n elements. */
static void ensure_cache(idx_t n)
{
    while (cache_size < n) {
        ++last_checked;
        if (is_center_v(last_checked))
            append_candidate(&cache, &cache_size, &cache_cap,
                             (seq_t)last_checked);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* unused, offset 1 */
    ensure_cache(n);
    return cache[n-1];
}
