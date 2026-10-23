/*
 * A015587  Expansion of x/(1 - 9*x - 11*x^2).
 * https://oeis.org/A015587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 11*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache holds already computed sequence values.
       len  – number of elements currently stored (>= 2 once initialized)  */
    static seq_t *cache = NULL;
    static size_t len = 0;

    /* Lazy initialization: ensure at least a[0] and a[1] exist. */
    if (len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (!cache) return 0;          /* fallback; unlikely to happen */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* Extend cache up to index n if necessary. */
    if ((size_t)n >= len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return 0;            /* fallback */
        cache = tmp;
        /* Compute new terms using the recurrence. */
        for (size_t i = len; i < new_len; ++i) {
            cache[i] = (seq_t)9 * cache[i - 1] + (seq_t)11 * cache[i - 2];
        }
        len = new_len;
    }

    return cache[(size_t)n];
}
