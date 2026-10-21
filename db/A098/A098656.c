/*
 * A098656  Expansion of x(1-4x)/((1-2x)(1-8x^2)).
 * https://oeis.org/A098656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) - 16*a(n-3) for n ≥ 3 with a(0)=0,
 *            a(1)=1, a(2)=−2 (derived from the generating function
 *            x(1-4x)/((1-2x)(1-8x²)))
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 21, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache      = NULL;   /* pointer to cache array          */
    static size_t  cache_len = 0;      /* number of terms actually stored */
    static size_t  cache_cap = 0;      /* allocated capacity              */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* store the first three terms explicitly */
        cache_cap = 8; /* small initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* fallback on memory failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-2;
        cache_len = 3;
    }

    /* grow cache if necessary */
    if (n >= cache_len) {
        /* ensure at least n+1 capacity */
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= (size_t)n + 1) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;           /* fallback on failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms up to n using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t term = 2 * cache[i - 1] + 8 * cache[i - 2] - 16 * cache[i - 3];
            cache[i] = term;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
