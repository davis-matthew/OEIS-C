/*
 * A135261  a(n) = 3*A131090(n) - A131090(n+1).
 * https://oeis.org/A135261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) Initial values: a(0) = -1,
 *            a(1) = 3, a(2) = -1, a(3) = 2
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 66, i128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes values to allow out‑of‑order queries. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed values */
    static idx_t cache_len = 0;   /* number of valid entries */

    /* Initialize cache with first four terms if not already done */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* memory allocation failed */
        cache[0] = (seq_t)(-1);
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)(-1);
        cache[3] = (seq_t)2;
    }

    /* If the requested index is already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Expand the cache to accommodate index n */
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* memory allocation failed */
    cache = tmp;

    /* Compute new values up to n using the recurrence relation */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = ((seq_t)2 * cache[i - 1])        /* 2*a(i-1) */
                    - cache[i - 3]                  /* - a(i-3) */
                    + ((seq_t)2 * cache[i - 4]);    /* + 2*a(i-4) */
        cache[i] = val;
    }

    /* Update cache length and return requested value */
    cache_len = n + 1;
    return cache[n];
}
