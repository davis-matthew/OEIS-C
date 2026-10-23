/*
 * A029084  Expansion of 1/((1-x)(1-x^4)(1-x^11)(1-x^12)).
 * https://oeis.org/A029084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 1, 4, 11, 12 (OEIS
 *            A029084). The implementation uses a simple coin‑change DP with
 *            parts [1,4,11,12] applied in fixed order, recomputing the cache
 *            only when a larger n is requested. The algorithm is O(k·n) per
 *            expansion, where k = 4 is the number of allo...
 * Range:     u8 n <= 79, u16 n <= 578, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */

    /* extend cache if necessary */
    if (n >= cache_size) {
        /* new cache size is n+1 to accommodate index n */
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));

        /* initialise the array for recomputation */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;
        cache[0] = 1;   /* empty sum */

        /* permitted part sizes */
        const idx_t parts[4] = {1, 4, 11, 12};

        /* DP: for each part add its contribution to all larger sums */
        for (int pi = 0; pi < 4; ++pi) {
            idx_t p = parts[pi];
            for (idx_t i = p; i < cache_size; ++i)
                cache[i] += cache[i - p];
        }
    }

    return cache[n];
}
