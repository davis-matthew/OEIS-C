/*
 * A230295  a(n) = Sum_{i=1..n} d(8*i+5) - Sum_{i=1..n} d(4*i+3), where d(n) =
 *          A000005(n).
 * https://oeis.org/A230295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} d(8*i+5) - Sum_{i=1..n} d(4*i+3) , where
 *            d(n) = A000005(n)
 * Range:     u8 n <= 706, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

static seq_t *a_cache = NULL;        /* stores a(1)..a(n) */
static idx_t  cache_size = 0;        /* number of stored elements */
static idx_t  cache_cap   = 0;        /* allocated capacity */
static seq_t  cum_D8 = 0;            /* cumulative sum of d(8*i+5) */
static seq_t  cum_D4 = 0;            /* cumulative sum of d(4*i+3) */

static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n<1, but return 0 for safety */
        return 0;
    if (n > cache_size) {
        ensure_capacity(n);
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            uint64_t val8 = 8 * i + 5;
            uint64_t val4 = 4 * i + 3;
            uint64_t d8 = divisor_count_u64(val8);
            uint64_t d4 = divisor_count_u64(val4);
            cum_D8 += (seq_t)d8;
            cum_D4 += (seq_t)d4;
            a_cache[i - 1] = cum_D8 - cum_D4; /* cum_D8 >= cum_D4 always */
        }
        cache_size = n;
    }
    return a_cache[n - 1];
}
