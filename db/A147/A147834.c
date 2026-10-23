/*
 * A147834  Coefficient expansion of the characteristic polynomial of the
 *          {3,4,5} simplex matrix: M = {{0, 3, 0}, {0, 0, 4}, {1, 1, 1}};
 *          p(x)=12 + 4 x + x^2 - x^3.
 * https://oeis.org/A147834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=5; a(n)=a(n-1)+4*a(n-2)+12*a(n-3) for n>2 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0; /* number of values stored so far (next index to compute) */
    static size_t cap = 0;       /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        max_cached = 3;  /* we have computed indices 0,1,2 */
    }

    /* if n already cached, return it */
    if (n < max_cached) {
        return cache[n];
    }

    /* grow cache if necessary */
    if ((idx_t)(cap) < n + 1) {
        size_t new_cap = cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0; /* allocation failed */
        cache = new_cache;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_cached; i <= n; ++i) {
        /* a(i) = a(i-1) + 4*a(i-2) + 12*a(i-3) */
        seq_t term = cache[i - 1]
                    + (seq_t)4 * cache[i - 2]
                    + (seq_t)12 * cache[i - 3];
        cache[i] = term;
    }
    max_cached = n + 1; /* updated number of cached terms */
    return cache[n];
}
