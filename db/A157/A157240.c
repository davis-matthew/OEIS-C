/*
 * A157240  a(n) = A128018(n) + 1.
 * https://oeis.org/A157240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-2) + 4*a(n-3) for n > 2 with initial
 *            values a(0) = 2, a(1) = -1, a(2) = -7 Sequence A157240: a(n) =
 *            A128018(n) + 1
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of already computed values */
    static idx_t cache_cap   = 0;        /* allocated capacity in elements   */
    static idx_t cache_last  = 0;        /* highest index that has been filled */

    /* Allocate cache on first call and initialise the first three values */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure – unlikely in tests */

        /* initial values */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-7;
        cache_last = 2;                  /* indices 0,1,2 are known */
    }

    /* For small n we can simply return from the cache */
    if (n <= cache_last)
        return cache[n];

    /* Expand capacity if the requested n exceeds it */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n, storing them in the cache */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1]
                 - (seq_t)6 * cache[i-2]
                 + (seq_t)4 * cache[i-3];
    }
    cache_last = n;
    return cache[n];
}
