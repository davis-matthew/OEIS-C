/*
 * A131244  Row sums of triangle A131243.
 * https://oeis.org/A131244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3) - a(n-4); with initial
 *            values a(0)=1, a(1)=3, a(2)=6, a(3)=14
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */

    /* initialise cache with first 4 terms on first call */
    if (!cache) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 6;
        cache[3] = 14;
    }

    /* if the requested term is already cached, return it directly */
    if (n < cache_len)
        return cache[n];

    /* extend cache to hold up to term n (inclusive) */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;                  /* allocation failure */
    cache = tmp;

    /* compute new terms using the recurrence relation */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t val = cache[i-1] + cache[i-1]          /* 2*a(i-1) */
                  + cache[i-2] + cache[i-2];         /* 2*a(i-2) */
        val -= cache[i-3] + cache[i-3] + cache[i-3] /* 3*a(i-3) */
              + cache[i-4];                         /*  a(i-4) */
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
