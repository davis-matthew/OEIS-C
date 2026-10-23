/*
 * A167375  a(n)=3*a(n-1)-a(n-2) with a(0)=1, a(1)=3, a(2)=11.
 * https://oeis.org/A167375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2), with a(0)=1, a(1)=3, a(2)=11
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    size_t i;

    /* Initialise the cache with the first three terms */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)11;
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute terms up to n */
    for (i = 3; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)(3 * cache[i - 1] - cache[i - 2]);
    }

    return cache[(size_t)n];
}
