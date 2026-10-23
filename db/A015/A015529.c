/*
 * A015529  Expansion of x/(1 - 3*x - 11*x^2).
 * https://oeis.org/A015529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 11*a(n-2), with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A015529 */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t cache_len  = 0;      /* number of elements stored (cache_len == 0 -> none) */

    /* Ensure we have space for terms up to index 'n' */
    if ((size_t)n + 1 > cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)   /* allocation failed – return 0 (should not happen in tests) */
            return 0;

        /* Initialize base cases if the cache is newly created or only has a(0) */
        if (cache_len == 0)
        {
            new_cache[0] = (seq_t)0;
            if (new_len > 1)
                new_cache[1] = (seq_t)1;
            /* Compute terms from index 2 up to 'n' */
            for (idx_t i = 2; i <= n; ++i)
                new_cache[i] = 3 * new_cache[i-1] + 11 * new_cache[i-2];
        }
        else
        {
            /* Cache already has values up to cache_len-1 */
            /* If cache_len == 1 then we only have a(0); need to set a(1) */
            if (cache_len == 1)
            {
                new_cache[1] = (seq_t)1;
                /* Start computing from index 2 */
                for (idx_t i = 2; i <= n; ++i)
                    new_cache[i] = 3 * new_cache[i-1] + 11 * new_cache[i-2];
            }
            else
            {
                /* cache_len >= 2 : we have a(0) and a(1) */
                for (idx_t i = cache_len; i <= n; ++i)
                    new_cache[i] = 3 * new_cache[i-1] + 11 * new_cache[i-2];
            }
        }

        cache     = new_cache;
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
