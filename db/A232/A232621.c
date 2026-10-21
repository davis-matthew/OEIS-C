/*
 * A232621  The number of vertically fault-free domino tilings of the 5 X (2n)
 *          board.
 * https://oeis.org/A232621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 4 for n >= 4 a(0) = 1, a(1) = 8, a(2)
 *            = 31, a(3) = 175 (Computed from the recurrence given in the OEIS
 *            entry)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n) {
    /* Static cache for memoisation across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of allocated elements */

    /* Resize the cache if the requested index is beyond current size */
    if (n >= cache_size) {
        idx_t new_size = n + 1;       /* need indices 0 .. n */
        seq_t *new_cache = seq_alloc_new_array(new_size);

        /* Copy existing values that fit into the new array */
        for (idx_t i = 0; i < cache_size && i < new_size; ++i) {
            new_cache[i] = cache[i];
        }

        /* Initialise the base values for small indices */
        if (new_size > 0) new_cache[0] = (seq_t)1;
        if (new_size > 1) new_cache[1] = (seq_t)8;
        if (new_size > 2) new_cache[2] = (seq_t)31;
        if (new_size > 3) new_cache[3] = (seq_t)175;

        /* Compute remaining values using the recurrence */
        idx_t start = cache_size > 4 ? cache_size : 4;  /* first index to fill */
        for (idx_t i = start; i <= n; ++i) {
            new_cache[i] = 6 * new_cache[i-1] - new_cache[i-2] - (seq_t)4;
        }

        /* Free old cache and update pointers */
        if (cache) seq_free(cache);
        cache       = new_cache;
        cache_size  = new_size;
    }

    return cache[n];
}
