/*
 * A063165  Dimension of the space of weight 2n cusp forms for Gamma_0( 97 ).
 * https://oeis.org/A063165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)+a(n-3)-a(n-5) for n > 6 with a(1) = 7, a(2) = 24,
 *            a(3) = 40, a(4) = 56, a(5) = 72, a(6) = 90
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; behaviour below 1 is ignored. */
    if (n < 1)
        return 0;

    /* Static cache: cache[i] holds a(i) for i >= 1. */
    static seq_t *cache = NULL;      /* 1‑based array, cache[0] unused. */
    static size_t cache_size = 0;    /* Initialized up to this index. */
    static size_t cache_cap = 0;     /* Allocated capacity. */

    /* If cache is empty, allocate initial capacity. */
    if (cache_cap == 0) {
        cache_cap = 8;                /* enough for indices up to 7. */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;         /* allocation failed */
        cache[0] = 0;                 /* dummy */
        /* initialise the first six terms */
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)24;
        cache[3] = (seq_t)40;
        cache[4] = (seq_t)56;
        cache[5] = (seq_t)72;
        cache[6] = (seq_t)90;
        cache_size = 6;
    }

    /* Expand cache if needed. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;           /* 1‑based index */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 6) {
            /* base terms already filled; these positions may be recomputed
               but the values are constants.                     */
            /* On first run cache[1..6] are set. Later calls skip
               recomputation because i > cache_size.             */
            continue;
        }
        /* recurrence: a(i) = a(i-2) + a(i-3) - a(i-5) */
        val = cache[i-2];
        val = val + cache[i-3];
        val = val - cache[i-5];
        cache[i] = val;
    }
    cache_size = (size_t)n;
    return cache[n];
}
