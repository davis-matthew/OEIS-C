/*
 * A037101  Trajectory of 3 under map n->7n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A037101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3; for n > 0, a(n) = (a(n-1) is even) ? a(n-1)/2 :
 *            7*a(n-1)+1
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 60, u64 n <= 203, u128 n <= 417,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* Grow cache to hold indices up to n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(1);           /* allocation failed */

        if (cache_size == 0) {
            /* initialise first term */
            new_cache[0] = (seq_t)3;
            cache_size = 1;
        }

        /* Compute new terms */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t prev = new_cache[i - 1];
            new_cache[i] = (prev & 1) ? (7 * prev + (seq_t)1) : (prev >> 1);
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
