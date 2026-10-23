/*
 * A016277  Expansion of 1/((1-2x)(1-3x)(1-8x)).
 * https://oeis.org/A016277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 46*a(n-2) + 48*a(n-3), a(0)=1, a(1)=13,
 *            a(2)=123
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values of the sequence   */
    static idx_t cache_len = 0;      /* number of terms stored in cache */

    /* Grow the cache when necessary */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;

        /* Allocate a larger buffer and copy old values */
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache) {
            for (idx_t i = 0; i < old_len; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cache_len = new_len;

        /* Initialise base terms (if not already present) */
        if (cache_len > 0) cache[0] = (seq_t)1;
        if (cache_len > 1) cache[1] = (seq_t)13;
        if (cache_len > 2) cache[2] = (seq_t)123;

        /* Compute new terms using the recurrence relation */
        idx_t start = (old_len > 3) ? old_len : 3;
        for (idx_t i = start; i < cache_len; ++i) {
            cache[i] = (seq_t)13 * cache[i-1]
                     - (seq_t)46 * cache[i-2]
                     + (seq_t)48 * cache[i-3];
        }
    }

    return cache[n];
}
