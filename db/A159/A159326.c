/*
 * A159326  Numerator of Hermite(n, 4/11).
 * https://oeis.org/A159326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 8*a(n-1) - 242*(n-1)*a(n-2) with a(0)=1, a(1)=8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence relation from OEIS A159326 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size */
    static idx_t last_idx = (idx_t)-1;  /* highest index that has been computed */

    /* --- initialise cache if this is the first call --- */
    if (cache == NULL)
    {
        cache_cap = n + 1;          /* at least store up to n */
        if (cache_cap < 2) cache_cap = 2;   /* need room for a(1) when n>=1 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure – return 0 as a fallback */

        /* seed initial values */
        cache[0] = 1;
        cache[1] = 8;
        last_idx = 1;
    }
    /* --- grow the cache if the requested index is outside current bounds --- */
    else if (n + 1 > cache_cap)
    {
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;    /* allocation failure – fallback to 0 */

        /* copy the already computed part */
        for (idx_t i = 0; i <= last_idx; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache = new_cache;
        cache_cap = n + 1;
    }

    /* --- compute missing values up to n using the recurrence --- */
    if (n > last_idx)
    {
        for (idx_t i = last_idx + 1; i <= n; ++i)
        {
            /* recurrence: a(i) = 8*a(i-1) - 242*(i-1)*a(i-2) */
            cache[i] = 8 * cache[i-1]
                       - 242 * (seq_t)(i-1) * cache[i-2];
        }
        last_idx = n;
    }

    /* Return the requested value */
    return cache[n];
}
