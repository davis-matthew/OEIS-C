/*
 * A159327  Numerator of Hermite(n, 5/11).
 * https://oeis.org/A159327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 10, a(n) = 10*a(n-1) - 242*(n-1)*a(n-2) for n >
 *            1 (recurrence)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;  /* array of computed values      */
    static idx_t  cache_len  = 0;     /* number of values stored       */
    static idx_t  cache_cap  = 0;     /* allocated capacity in array    */

    /* initialise cache with a(0) and a(1) */
    if (cache == NULL) {
        cache_cap = 2;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)   /* memory allocation failure */
            return (seq_t)0; /* caller may detect non‑zero return          */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache_len = 2;
    }

    /* small indices handled directly */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* grow the cache if necessary */
    if (cache_len <= n) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure          */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev1 = cache[i-1];            /* a(i-1)   */
        seq_t prev2 = cache[i-2];            /* a(i-2)   */
        seq_t term1 = (seq_t)10 * prev1;                         /* 10*a(i-1) */
        seq_t term2 = (seq_t)242 * (seq_t)(i-1) * prev2;          /* 242*(i-1)*a(i-2) */
        cache[i] = term1 - term2;                                /* recurrence */
    }

    cache_len = n + 1;
    return cache[n];
}
