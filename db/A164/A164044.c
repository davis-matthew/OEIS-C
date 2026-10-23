/*
 * A164044  a(n+1) = 4*a(n) - n.
 * https://oeis.org/A164044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n+1) = 4*a(n) - (n+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to reuse previously computed values */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;                            /* store a(0) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)  /* allocation failed */
            return (seq_t)0;
        cache[0] = (seq_t)1;  /* a(0) = 1 */
    }

    /* extend the cache if the requested index is outside the current range */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {          /* allocation failed */
            free(cache);
            cache = NULL;
            cache_cap = 0;
            return (seq_t)0;
        }
        cache = tmp;

        /* compute all missing terms using the recurrence */
        for (size_t i = cache_cap; i <= (size_t)n; ++i) {
            cache[i] = 4 * cache[i - 1] - (seq_t)i;
        }
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
