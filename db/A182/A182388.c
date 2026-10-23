/*
 * A182388  a(0) = 1, a(n) = (a(n-1) XOR n) + n.
 * https://oeis.org/A182388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (a(n-1) XOR n) + n, with a(0) = 1
 * Range:     u8 n <= 27, u16 n <= 349, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cached sequence.  cache[0] == a(0) == 1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored */
    static idx_t cache_cap  = 0;   /* allocated capacity (in elements) */

    /* Allocate cache on first invocation */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Store the initial value */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Extend the cache if the requested index is beyond the current size */
    if (n >= cache_size) {
        /* Grow the array if necessary */
        idx_t needed = n + 1;
        if (needed > cache_cap) {
            idx_t new_cap = cache_cap;
            while (new_cap < needed) new_cap <<= 1;   /* multiply by 2 */
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache_cap = new_cap;
        }

        /* Compute missing terms iteratively */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t i_seq = (seq_t)i;
            seq_t prev  = cache[i - 1];
            cache[i] = (prev ^ i_seq) + i_seq;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
