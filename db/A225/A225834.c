/*
 * A225834  Number of binary pattern classes in the (10,n)-rectangular grid:
 *          two patterns are in the same class if one of them can be obtained
 *          by a reflection or 180-degree rotation of the other.
 * https://oeis.org/A225834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^10*a(n-1) + 2^10*a(n-2) - (2^10)^2*a(n-3) a(0)=1,
 *            a(1)=528, a(2)=262912 Recurrence holds for n>2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   free_cache, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* Cache of already computed terms.
       cache_len is the number of terms stored (highest index + 1). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialise cache with the first three terms if not already done. */
    if (!cache)
    {
        cache = seq_alloc_new_array(3);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)528;
        cache[2] = (seq_t)262912;
        cache_len = 3;
    }

    /* If the requested index is beyond the current cache,
       grow the cache and compute the missing terms. */
    if ((size_t)n + 1 > cache_len)
    {
        size_t old_len   = cache_len;
        size_t new_len   = (size_t)n + 1;
        seq_t *new_cache = seq_alloc_new_array(new_len);

        /* Copy existing values to the new cache. */
        memcpy(new_cache, cache, old_len * sizeof(seq_t));

        /* Free old cache and adopt the new one. */
        free_cache(cache);
        cache = new_cache;
        cache_len = new_len;

        /* Compute new terms using the recurrence. */
        for (idx_t i = old_len; i <= n; ++i)
        {
            seq_t t1 = cache[i - 1] << 10;  /* 2^10 * a(i-1) */
            seq_t t2 = cache[i - 2] << 10;  /* 2^10 * a(i-2) */
            seq_t t3 = cache[i - 3] << 20;  /* (2^10)^2 * a(i-3) */
            cache[i] = t1 + t2 - t3;
        }
    }

    /* The result is now in the cache. */
    return cache[n];
}
