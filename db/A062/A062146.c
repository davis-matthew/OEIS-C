/*
 * A062146  Row sums of signed triangle A062137 (generalized Laguerre, a=3).
 * https://oeis.org/A062146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n+1)*a(n-1) - (n-1)*(n+2)*a(n-2) with a(0) = 1, a(1) =
 *            3
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*/**/ static seq_t *cache = NULL; /* dynamically grown cache */
    /*/**/ static idx_t cache_len = 0; /* number of valid entries in cache */

    /* initialise cache with a(0) and a(1) */
    if (cache == NULL) {
        cache_len = 2;
        /* allocate space for at least two elements */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0; /* out‑of‑memory: caller will treat as 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* grow the cache up to size n+1 */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return 0; /* out‑of‑memory */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms iteratively using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        /* Compute (2*i+1) and (i-1)*(i+2) as idx_t to avoid
           overflows on very small seq_t types, then cast to seq_t. */
        idx_t factor1_idx = (idx_t)2 * i + 1;
        idx_t factor2_idx = (idx_t)(i - 1) * (idx_t)(i + 2);
        seq_t factor1 = (seq_t)factor1_idx;
        seq_t factor2 = (seq_t)factor2_idx;

        /* term1 = factor1 * a(i-1) */
        seq_t term1 = factor1 * cache[i - 1];
        /* term2 = factor2 * a(i-2) */
        seq_t term2 = factor2 * cache[i - 2];

        cache[i] = term1 - term2;
    }

    return cache[n];
}
