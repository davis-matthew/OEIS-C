/*
 * A129988  Fourth differences of A129983.
 * https://oeis.org/A129988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) -13*a(n-2) +12*a(n-3) -3*a(n-4) -2*a(n-5), n>5,
 *            with initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=9, a(4)=20,
 *            a(5)=44
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: length of filled entries, capacity, and array pointer */
    static seq_t *cache = NULL;
    static size_t cap = 0, len = 0;

    /* initialize cache on first call */
    if (cache == NULL) {
        cap = 16;                /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failed; return 0 as fallback */

        /* store the first six terms */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 9;
        cache[4] = 20;
        cache[5] = 44;
        len = 6;
    }

    /* if the request is already computed, just return it */
    if (n < len)
        return cache[n];

    /* ensure the cache can hold up to n */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failed; fall back */
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)6 * cache[i - 1]
                 - (seq_t)13 * cache[i - 2]
                 + (seq_t)12 * cache[i - 3]
                 - (seq_t)3  * cache[i - 4]
                 - (seq_t)2  * cache[i - 5];
    }
    len = n + 1;
    return cache[n];
}
