/*
 * A116760  Number of permutations of length n which avoid the patterns 2341,
 *          3214, 4213; or avoid the patterns 1324, 2341, 3214.
 * https://oeis.org/A116760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A116760: Recurrence 8a(n-1) - 23a(n-2) + 28a(n-3) - 13a(n-4) + 2a(n-5)
   with initial values a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Compute a(n) for n >= 1.  Cached values are stored in a static array
   so that calls can be in any order.  The recurrence is used for n >= 6.
   Initial values up to n = 5 are returned from the lookup table. */
seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;     /* array of a(1) .. a(max) */
    static idx_t cache_cap = 0;      /* allocated capacity */
    static idx_t cache_len = 0;      /* number of valid terms stored */

    /* initial values for n = 1..5 */
    const seq_t init[5] = {1, 2, 6, 21, 73};

    /* if cache has not been allocated yet, allocate the minimum size */
    if (cache == NULL)
    {
        /* we need at least capacity for the requested n or for 5, whichever is larger */
        cache_cap = n > 5 ? n : 5;
        cache = alloc_seq_array(cache_cap);

        /* store the first five known terms */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init[i];

        cache_len = 5;  /* we have terms up to a(5) ready */
    }

    /* if the caller wants a term that is not yet available, grow the cache */
    if (n > cache_cap)
    {
        /* allocate a larger array and copy old values */
        seq_t *new_cache = alloc_seq_array(n);
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        free_seq_array(cache);
        cache = new_cache;
        cache_cap = n;
    }

    /* return a known term immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* compute missing terms up to n using the recurrence
       a(i) = 8a(i-1) – 23a(i-2) + 28a(i-3) – 13a(i-4) + 2a(i-5)   */
    for (idx_t i = cache_len; i < n; ++i)
    {
        cache[i] = cache[i - 1] * 8
                 - cache[i - 2] * 23
                 + cache[i - 3] * 28
                 - cache[i - 4] * 13
                 + cache[i - 5] * 2;
    }

    cache_len = n;  /* we now have terms up to a(n) stored */
    return cache[n - 1];
}
