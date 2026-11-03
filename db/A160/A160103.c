/*
 * A160103  Numerator of Hermite(n, 4/27).
 * https://oeis.org/A160103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=8, a(n+1)=8*a(n)-1458*n*a(n-1) (n≥1)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 355 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cached_len = 0;   /* number of terms currently stored */

    /* if the desired term is already cached, return it */
    if (n < cached_len)
        return cache[n];

    /* initialise cache to contain a(0) and a(1) */
    if (cached_len == 0U) {
        /* allocate space for two initial terms */
        cache = mem_resize(seq_t, cache, (size_t)2);
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)8;   /* a(1) */
        cached_len = 2U;
    }

    /* extend the cache up to index n */
    if (n >= cached_len) {
        /* grow the array to hold indices 0 … n */
        cache = mem_resize(seq_t, cache, (size_t)(n + 1));

        for (idx_t i = cached_len; i <= n; ++i) {
            /* recurrence: a(i) = 8*a(i-1) - 1458*(i-1)*a(i-2) */
            seq_t term  = cache[i - 1];              /* a(i-1) */
            term *= (seq_t)8;                        /* 8*a(i-1) */

            seq_t prod  = cache[i - 2];              /* a(i-2) */
            prod *= (seq_t)(i - 1);                  /* (i-1)*a(i-2) */
            prod *= (seq_t)1458;                     /* 1458*(i-1)*a(i-2) */

            cache[i] = term - prod;                  /* a(i) */
        }
        cached_len = n + 1U;
    }

    return cache[n];
}
