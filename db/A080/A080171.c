/*
 * A080171  a(n) = n*a(n-1) - (n-1)^2*a(n-2), a(0)=1, a(1)=1.
 * https://oeis.org/A080171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1,
   a(n)=n*a(n-1)-(n-1)^2*a(n-2)   (n≥2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored terms              */
    static idx_t cache_cap = 0;   /* capacity of the array                */

    /* Initialise cache with the first two terms. */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* If the requested term is already computed, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache until it can accommodate n. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms sequentially. */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t term1 = (seq_t)k * cache[k - 1];
        seq_t km1 = (seq_t)(k - 1);
        seq_t term2 = km1 * km1 * cache[k - 2];
        cache[k] = term1 - term2;
    }

    cache_len = n + 1;
    return cache[n];
}
