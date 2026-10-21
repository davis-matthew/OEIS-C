/*
 * A077846  Expansion of g.f. 1/(1 - 3*x + 2*x^3).
 * https://oeis.org/A077846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-3), a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation – uninitialised until first call */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of elements currently stored */

    /* increase cache size if required */
    if (n >= cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *new_cache = alloc_cache(new_len);   /* *alloc_cache* creates an array of seq_t of the requested size */

        /* copy previously computed values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* compute new values from the first missing index up to n */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)                  new_cache[i] = (seq_t)1;
            else if (i == 1)             new_cache[i] = (seq_t)3;
            else if (i == 2)             new_cache[i] = (seq_t)9;
            else                         new_cache[i] = 3 * new_cache[i - 1] - 2 * new_cache[i - 3];
        }

        /* free old cache if it existed */
        if (cache)
            free_cache(cache);

        cache     = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
