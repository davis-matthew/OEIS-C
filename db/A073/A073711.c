/*
 * A073711  G.f. A(x) satisfies A(x) = A(x^2) + x*A(x^2)^2.
 * https://oeis.org/A073711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(0)=1; a(2^k)=1; a(2^k*n)=a(n); a(2n+1)=Σ_{k=0}^n a(k)*a(n-k)
 * Range:     u8 n <= 32, u16 n <= 92, u32 n <= 284, u64 n <= 988,
 *            u128 n <= 3620, big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build.  The helper function
 * alloc_cache allocates an array of seq_t of the requested length.
 * It is defined in the supplied utilities. */
#include "../utils/alloc_cache.h"

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms.  cache_len is the
     * size of the cache; values are stored in cache[0..cache_len-1].  */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Grow the cache to at least n+1 entries. */
    if (n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = alloc_cache(new_len);   /* helper knows how to
                                                    * construct seq_t objects   */
        /* Copy old values, if any.  (alloc_cache returns freshly
           constructed objects, so copying is safe.) */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Compute new entries from cache_len up to n. */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
            {
                new_cache[i] = (seq_t)1;
                continue;
            }

            /* Even indices.  If i is a power of two then a(i) = 1.
               Otherwise remove the trailing zeros (divide by 2^v2(i))
               and reuse the cached value of the odd part. */
            if ((i & 1) == 0)
            {
                if ((i & (i - 1)) == 0)   /* power of two */
                {
                    new_cache[i] = (seq_t)1;
                }
                else
                {
                    idx_t odd = i;
                    while ((odd & 1) == 0)
                        odd >>= 1;
                    new_cache[i] = new_cache[odd];
                }
                continue;
            }

            /* i is odd and >=1.  Special case a(1)=1. */
            if (i == 1)
            {
                new_cache[i] = (seq_t)1;
                continue;
            }

            /* Odd index: use the convolution recurrence.
               Write i = 2*m+1, so m = (i-1)/2.  */
            idx_t m   = (i - 1) >> 1;
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= m; ++k)
                sum += new_cache[k] * new_cache[m - k];
            new_cache[i] = sum;
        }

        /* Replace old cache.  We do not shrink the allocation.  */
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
