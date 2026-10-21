/*
 * A077985  Expansion of 1/(1 + 2*x - x^2).
 * https://oeis.org/A077985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -2, a(n) = -2*a(n-1) + a(n-2) for n > 1
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for OEIS A077985: Expansion of 1/(1 + 2*x - x^2). */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;   /* array of a(0), a(1), ... */
    static idx_t cache_len = 0;   /* number of elements stored in cache */

    /* initialize cache if this is the first call */
    if (cache == NULL)
    {
        /* allocate space for at least two terms */
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        cache[0] = (seq_t)1;            /* a(0) */
        cache[1] = (seq_t)-2;           /* a(1) */
        cache_len = 2;
    }

    /* if we already have the requested term, return it */
    if (n < cache_len)
        return cache[n];

    /* extend the cache to hold up to a(n) */
    idx_t new_len = n + 1;              /* number of terms needed */
    if (new_len > cache_len)
    {
        /* realloc with the larger size */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
        {
            /* allocation failed; return 0 (caller will treat as invalid) */
            return (seq_t)0;
        }
        cache = tmp;

        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            cache[i] = -2 * cache[i - 1] + cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
