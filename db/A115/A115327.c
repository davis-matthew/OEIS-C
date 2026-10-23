/*
 * A115327  E.g.f.: exp(x + 3/2*x^2).
 * https://oeis.org/A115327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(n)=a(n-1)+3*(n-1)*a(n-2) for n>=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* pointer to array holding computed terms */
    static size_t cache_cap = 0;  /* allocated size of the cache (in elements) */
    static idx_t cache_size = 0;  /* number of terms currently stored (>= cache_cap) */

    /* Initialise cache for n = 0 and n = 1 */
    if (!cache)
    {
        cache_cap  = 2;
        /* Allocate an array of seq_t of length cache_cap */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Grow the cache if needed */
    if ((size_t)(n + 1) > cache_cap)
    {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;          /* allocation failure */
        cache      = new_cache;
        cache_cap  = (size_t)(n + 1);
        /* cache_size already holds the number of stored terms; it does not change here */
    }

    /* Compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* i >= 2 because cache_size is at least 2 */
        seq_t coeff = (seq_t)(3 * (i - 1));          /* 3*(i-1) as seq_t */
        seq_t val   = cache[i - 1] + coeff * cache[i - 2];
        cache[i]    = val;
    }

    cache_size = (idx_t)(n + 1);
    return cache[n];
}
