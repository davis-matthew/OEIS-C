/*
 * A025982  Expansion of 1/((1-2*x)*(1-4*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A025982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 242*a(n-2) + 816*a(n-3) - 864*a(n-4) with
 *            a(0)=1, a(1)=27, a(2)=487, a(3)=7431
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is initialized with the first four terms */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0; /* failed allocation */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)487;
        cache[3] = (seq_t)7431;
    }

    /* Return known values immediately */
    if (n < cache_size)
        return cache[n];

    /* Grow cache to accommodate index n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) return 0; /* failed allocation */
    cache = new_cache;

    /* Compute new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = (seq_t)27 * cache[i-1];
        seq_t t2 = (seq_t)242 * cache[i-2];
        seq_t t3 = (seq_t)816 * cache[i-3];
        seq_t t4 = (seq_t)864 * cache[i-4];
        /* Compute: t1 + t3 - (t2 + t4)  */
        seq_t sum1 = t1 + t3;
        seq_t sum2 = t2 + t4;
        cache[i] = sum1 - sum2;
    }
    cache_size = n + 1;

    return cache[n];
}
