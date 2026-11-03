/*
 * A048421  Squares composed of two '1-digit' overlapping subsquares.
 * https://oeis.org/A048421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A048421:
   squares whose decimal representation can be split into an overlapping
   pair of perfect squares, each at least two digits long, with no
   leading or trailing zeros in the subsquares. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdint.h>
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

static seq_t *cache = NULL;
static size_t cacheSize = 0;
static size_t cacheCap = 0;
static idx_t lastChecked = 0;

static bool is_square(const seq_t x)
{
    seq_t r = isqrt_seq(x);
    return r * r == x;
}

static bool has_property(const seq_t sq)
{
    char buf[512];
    size_t len = 0;
    seq_to_str(sq, buf, &len);
    if (len < 3) return false;          /* need at least 2+2-1 digits */

    for (size_t a = 2; a <= len - 1; ++a) {
        if (buf[0] == '0'              /* u leading zero */
            || buf[a - 1] == '0'      /* u trailing / v leading zero */
            || buf[len - 1] == '0')   /* v trailing zero */
            continue;

        /* Parse u */
        seq_t u = 0;
        for (size_t i = 0; i < a; ++i)
            u = u * 10 + (buf[i] - '0');

        /* Parse v */
        seq_t v = 0;
        for (size_t i = a - 1; i < len; ++i)
            v = v * 10 + (buf[i] - '0');

        if (is_square(u) && is_square(v))
            return true;
    }
    return false;
}

static void push_term(seq_t val)
{
    if (cacheSize == cacheCap) {
        size_t newCap = cacheCap ? cacheCap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, cache, newCap);
        if (!tmp) exit(1);
        cache = tmp;
        cacheCap = newCap;
    }
    cache[cacheSize++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for index 0, safe return */

    while (cacheSize < (size_t)n) {
        ++lastChecked;
        seq_t w = (seq_t)lastChecked;
        seq_t sq = w * w;
        if (has_property(sq))
            push_term(sq);
    }
    return cache[n - 1];
}
