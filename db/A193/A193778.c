/*
 * A193778  Number of signed permutations of length 2n invariant under D and
 *          D'bar.
 * https://oeis.org/A193778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*(n-1)*a(n-2) a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 649 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache variables */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t maxComputed = 0;   /* largest index that has been filled */

    /* Initialise cache with the first two terms */
    if (!cache) {
        cache_size = 2;
        cache = mem_new(seq_t, cache_size);
        if (!cache) abort();          /* out of memory */
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)2;          /* a(1) */
        maxComputed = 1;
    }

    /* Grow the cache if we need a larger value */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    while (maxComputed < n) {
        idx_t i = maxComputed + 1;               /* next index to compute */
        seq_t term = (seq_t)2 * cache[i - 1]
                   + (seq_t)4 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term;
        ++maxComputed;
    }

    return cache[n];
}
