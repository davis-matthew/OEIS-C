/*
 * A049017  Expansion of 1/((1-x)^7 - x^7).
 * https://oeis.org/A049017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/((1 - x)^7 - x^7); a(0..6) = binom(n+6, 6)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   binom_nplus6_6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom_nplus6_6.h"

static seq_t *cache = NULL;    /* cached values a(0)...a(cache_size-1) */
static idx_t  cache_size = 0;  /* number of values currently stored */
static size_t cache_cap   = 0;  /* allocated length of cache */

/* ensure cache has at least 'needed' slots; grows by doubling */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap < (size_t)needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)needed)
            new_cap *= 2;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) abort();   /* no sufficient memory – terminate */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_arr[i] = cache[i];
            free(cache);
        }
        cache  = new_arr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* we need to compute a(0)..a(n) (in any order of calls) */
    ensure_capacity(n + 1);

    /* compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = binom_nplus6_6(i);
        } else {
            /* a(i) = 7*a(i-1) -21*a(i-2) + 35*a(i-3) -35*a(i-4) +21*a(i-5) -7*a(i-6) + 2*a(i-7) */
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            seq_t t5 = cache[i - 5];
            seq_t t6 = cache[i - 6];
            seq_t t7 = cache[i - 7];
            cache[i] = 7 * t1 - 21 * t2 + 35 * t3 - 35 * t4 + 21 * t5 - 7 * t6 + 2 * t7;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
