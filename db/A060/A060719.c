/*
 * A060719  a(0) = 1; a(n+1) = a(n) + Sum_{i=0..n} binomial(n,i)*(a(i)+1).
 * https://oeis.org/A060719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n+1) = a(n) + Sum_{i=0..n} binomial(n,i)*(a(i)+1) (n
 *            ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
/* Utility functions that can be used */
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;      /* array of a(i) values      */
    static size_t  cache_cap = 0;    /* capacity of the array    */
    static idx_t   cache_size = 0;   /* number of values stored  */

    /* Extend cache if necessary */
    if (n >= cache_size)
    {
        /* Allocate or grow the cache to hold indices up to n */
        if (!ensure_cache) return 0;  /* safety check – shouldn't happen */
        ensure_cache(n, &cache, &cache_cap);

        /* Compute new terms from cache_size up to n */
        for (idx_t k = cache_size; k <= n; ++k)
        {
            if (k == 0)
            {
                cache[0] = (seq_t)1;
                continue;
            }

            /* Compute S = Sum_{i=0..k-1} binomial(k-1,i)*(a(i)+1) */
            seq_t sum = 0;
            idx_t top = k - 1;
            for (idx_t i = 0; i <= top; ++i)
            {
                seq_t coeff = binomial(top, i);         /* binomial coefficient */
                seq_t term  = coeff * (cache[i] + 1);   /* (a(i)+1) multiplied */
                sum += term;                            /* add to the sum      */
            }

            /* a(k) = a(k-1) + S */
            cache[k] = cache[k - 1] + sum;
        }

        cache_size = n + 1;
    }

    return cache[n];
}
