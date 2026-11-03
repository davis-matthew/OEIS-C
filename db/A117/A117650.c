/*
 * A117650  Heptagonal numbers for which the sum of the digits is also a
 *          heptagonal number.
 * https://oeis.org/A117650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 3554, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 55 terms through n = 9581 (2026-11-03)
 * Helpers:   digit_sum, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Heptagonal numbers H_k = k*(5k-3)/2 whose digit sum is also
   a heptagonal number.  The function returns the nth such number (a(0)=0). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return true if x is a heptagonal number.  Handles x = 0 specially. */
static bool is_heptagonal(seq_t x)
{
    if (x == 0)
        return true;
    /* Solve 5k^2 - 3k - 2x = 0.  The discriminant is D = 9 + 40x. */
    seq_t D = (seq_t)9 + (seq_t)40 * x;
    seq_t s = floor_sqrt(D);
    if (s * s != D)
        return false;
    return ((s + (seq_t)3) % (seq_t)10) == 0;
}

seq_t a(idx_t n)
{
    /* Static cache to accumulate qualifying heptagonal numbers. */
    static seq_t *cache      = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static idx_t   last_k    = 0;    /* next k to test */

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n+1 entries. */
    while (cache_len <= n) {
        seq_t k = (seq_t)last_k;
        seq_t H = k * (((seq_t)5 * k) - (seq_t)3) / (seq_t)2;

        seq_t dsum = digit_sum(H);
        if (is_heptagonal(dsum)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2 + 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = H;
        }
        last_k++;
    }
    return cache[n];
}
