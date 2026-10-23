/*
 * A023358  Number of compositions into sums of cubes.
 * https://oeis.org/A023358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1, k³ ≤ n} a(n – k³), with a(0) = 1
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 109, u64 n <= 215,
 *            u128 n <= 426, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;            /* array of a[0 .. cache_max]          */
    static size_t cache_cap = 0;           /* allocated size of cache array      */
    static idx_t cache_max = (idx_t)-1;    /* highest index that has been computed */

    /* initialise cache on first call */
    if (cache_max == (idx_t)-1)
    {
        cache = alloc_seq_array(1);
        if (!cache) return (seq_t)0;  /* allocation failure (unlikely) */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_cap = 1;
        cache_max = 0;
    }

    /* if the requested value is already known, just return it */
    if (n <= cache_max)
        return cache[n];

    /* resize the cache array if necessary */
    if ((size_t)(n + 1) > cache_cap)
    {
        seq_t *new_cache = alloc_seq_array((size_t)(n + 1));
        if (!new_cache) return (seq_t)0; /* allocation failure */
        for (idx_t i = 0; i <= cache_max; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = (size_t)(n + 1);
    }

    /* compute all missing values from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j * j * j <= i; ++j)
        {
            idx_t cube = j * j * j;
            sum += cache[i - cube];
        }
        cache[i] = sum;
    }

    cache_max = n;
    return cache[n];
}
