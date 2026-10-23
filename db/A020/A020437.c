/*
 * A020437  Numbers k such that the continued fraction for sqrt(k) has period
 *          98.
 * https://oeis.org/A020437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 318, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose continued fraction
 * for sqrt(k) has period 98.
 * The OEIS sequence A020437 is defined by that condition. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/append_candidate.h"

/* integer square root for 64-bit unsigned integers */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0;
    uint64_t high = 1ULL << 32;      /* sqrt(UINT64_MAX) < 2^32 */
    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        if ( ( (__int128)mid * mid) <= x )
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* test whether a number is a perfect square */
static inline bool is_square(uint64_t x)
{
    uint64_t r = isqrt_u64(x);
    return r * r == x;
}

/* compute the period of the continued fraction for sqrt(n)
 * return 0 if n is a perfect square.
 * If the period exceeds 98 the function stops early and returns >98. */
static inline size_t cf_period(uint64_t n)
{
    if (is_square(n)) return 0;
    uint64_t a0 = isqrt_u64(n);
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    size_t len = 0;
    while (1) {
        /* m = d*a - m */
        __int128 dm = (__int128)d * a - m;
        m = (uint64_t)dm;

        /* d = (n - m^2)/d */
        __int128 num = (__int128)n - (__int128)m * m;
        d = (uint64_t)(num / d);

        /* a = floor((a0 + m)/d) */
        a = (a0 + m) / d;

        ++len;
        if (a == 2 * a0)
            break;
        if (len > 98)   /* early bailout */
            break;
    }
    return len;
}

static seq_t  *period98_cache = NULL;
static size_t period98_len   = 0;
static size_t period98_cap   = 0;
static uint64_t period98_next = 2;   /* search starts from 2 */

/* Public API required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, harmless */
    /* precompute as many elements as needed */
    while (period98_len < (size_t)n) {
        /* find next integer k with period 98 */
        while (true) {
            uint64_t k = period98_next++;
            if (is_square(k))
                continue;
            if (cf_period(k) == 98) {
                append_candidate(&period98_cache, &period98_len,
                                 &period98_cap, (seq_t)k);
                break;
            }
        }
    }
    return period98_cache[n-1];
}
