/*
 * A165793  a(0)=1, a(n)=n*(a(n-1)-2).
 * https://oeis.org/A165793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(n)=n*(a(n-1)-2) for n>0
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated size */
    static idx_t  max_computed = (idx_t)-1;    /* largest n that has been computed */

    /* ensure the cache has enough space for index n */
    if ((size_t)n + 1 > cache_cap) {
        /* grow the cache: double the requested size for amortised O(1) */
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failed – value will be undefined */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first term if this is the first call */
    if (max_computed == (idx_t)-1) {
        cache[0] = 1;
        max_computed = 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)i * (cache[i - 1] - (seq_t)2);
    }
    max_computed = n;

    return cache[n];
}
