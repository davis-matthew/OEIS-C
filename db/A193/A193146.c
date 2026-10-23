/*
 * A193146  Expansion of 1/(1 - x - x^2 + x^3 - x^4 + x^6).
 * https://oeis.org/A193146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-6) (n ≥ 0), *
 *            initial: a(0) = 1 ; a(n) = 0 for n < 0
 * Range:     u8 n <= 16, u16 n <= 33, u32 n <= 68, u64 n <= 137,
 *            u128 n <= 274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store previously computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t len = 0; /* number of elements currently stored */

    if ((size_t)n < len)
        return cache[(size_t)n];      /* already computed */

    size_t needed = (size_t)n + 1;     /* we need indices 0..n */

    /* grow the cache if necessary */
    if (capacity == 0) {
        capacity = needed > 16 ? needed : 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    } else if (capacity < needed) {
        capacity = capacity * 2;
        if (capacity < needed)
            capacity = needed;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* compute missing values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 1;
        } else if (i == 2) {
            cache[2] = 2;
        } else if (i == 3) {
            cache[3] = 2;
        } else if (i == 4) {
            cache[4] = 4;
        } else if (i == 5) {
            cache[5] = 5;
        } else {
            /* recurrence a(i) = a(i-1)+a(i-2)-a(i-3)+a(i-4)-a(i-6) */
            seq_t val = cache[i-1] + cache[i-2];
            val -= cache[i-3];
            val += cache[i-4];
            val -= cache[i-6];
            cache[i] = val;
        }
    }

    len = (size_t)n + 1;
    return cache[(size_t)n];
}
