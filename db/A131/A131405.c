/*
 * A131405  Row sums of triangle A131404.
 * https://oeis.org/A131405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 3*a(n-3) + 3*a(n-4) - 2*a(n-5); *
 *            Initial values: a(0)=1, a(1)=2, a(2)=7, a(3)=16, a(4)=37
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdio.h>   /* for abort() */

seq_t a(idx_t n)
{
    /* Static cache for memoisation ---------------------------------------- */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with the first five terms on first call ------------- */
    if (cache == NULL) {
        cache_len = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)37;
    }

    /* If the requested value is already computed, just return it ----------- */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache to accommodate values up to n ------------------------ */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) abort();
    cache = tmp;

    /* Compute all new terms using the recurrence -------------------------- */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t val = 5 * cache[i - 1]      /* 5*a(i-1) */
                  - 8 * cache[i - 2]      /* -8*a(i-2) */
                  + 3 * cache[i - 3]      /* +3*a(i-3) */
                  + 3 * cache[i - 4]      /* +3*a(i-4) */
                  - 2 * cache[i - 5];     /* -2*a(i-5) */
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
