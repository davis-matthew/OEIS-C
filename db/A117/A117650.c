/*
 * A117650  Heptagonal numbers for which the sum of the digits is also a
 *          heptagonal number.
 * https://oeis.org/A117650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 3554, u64 n <= 9581,
 *            u128 n <= 9581, big any n
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Heptagonal numbers H_k = k*(5k-3)/2 whose digit sum is also
   a heptagonal number.  The function returns the nth such number (a(0)=0). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/digit_sum.h"

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
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n+1 entries. */
    while (cache_len <= n) {
        seq_t k = (seq_t)last_k;
        seq_t H = k * (((seq_t)5 * k) - (seq_t)3) / (seq_t)2;

        seq_t dsum = digit_sum(H);
        if (is_heptagonal(dsum)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2 + 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = H;
        }
        last_k++;
    }
    return cache[n];
}
