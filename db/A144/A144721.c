/*
 * A144721  a(0) = 2, a(1) = 5, a(n) = 4 * a(n-1) - a(n-2).
 * https://oeis.org/A144721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=5, a(n)=4*a(n-1)-a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* A cache that grows as needed.  The cache size is the next
       available index; cache[n] holds a(n) for all n < cache_size. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise the first two terms. */
    if (cache_size == 0) {
        cache = alloc_seq_array(2);
        if (!cache) abort(); /* allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache_size = 2;
    }

    /* If the requested index is beyond the current cache, grow it. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = alloc_seq_array(new_size);
        if (!new_cache) abort();

        /* Copy old values and compute new ones. */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        for (idx_t i = cache_size; i < new_size; ++i)
            new_cache[i] = (seq_t)4 * new_cache[i - 1] - new_cache[i - 2];

        free_seq_array(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
