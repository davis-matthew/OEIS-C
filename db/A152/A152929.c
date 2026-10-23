/*
 * A152929  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defining a polygonal configuration consisting of
 *          two 4-gonal polygonal components chained with string components of
 *          length l as l varies.
 * https://oeis.org/A152929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(1) = 113, a(2) = 176
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* make sure the cache can hold up to index n */
    if (n > cache_size)
    {
        idx_t new_size = n;
        seq_t *new_cache = seq_alloc_array(new_size + 1); /* +1 to use 1‑based index */
        if (!new_cache)
            return (seq_t)0; /* allocation failed – return 0 as safe default */

        /* copy existing values if any */
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];

        /* initialise new entries */
        for (idx_t i = cache_size + 1; i <= new_size; ++i)
        {
            if (i == 1)
                new_cache[i] = (seq_t)113;
            else if (i == 2)
                new_cache[i] = (seq_t)176;
            else
                new_cache[i] = new_cache[i - 1] + new_cache[i - 2];
        }

        /* free old cache */
        if (cache)
            seq_free_array(cache);

        cache     = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
