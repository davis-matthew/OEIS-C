/*
 * A077417  Chebyshev T-sequence with Diophantine property.
 * https://oeis.org/A077417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - a(n-2), a(0) = 1, a(1) = 11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache: a[0] = 1, a[1] = 11. */
    static seq_t *cache = NULL;
    static size_t len  = 0;      /* number of computed elements   */
    static size_t cap  = 0;      /* current allocation capacity   */

    if (!cache) {    /* initialise on first call */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)11;         /* a(1) */
        len  = 2;
    }

    /* Grow the cache until it contains a(n) */
    while (len <= (size_t)n) {
        /* Reallocate if needed */
        if (len == cap) {
            cap *= 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        /* Compute a(len) = 12*a(len-1) - a(len-2) */
        seq_t prev  = cache[len - 1];
        seq_t prev2 = cache[len - 2];
        cache[len] = (seq_t)12 * prev - prev2;
        len++;
    }

    return cache[(size_t)n];
}
