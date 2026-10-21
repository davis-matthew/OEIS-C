/*
 * A200441  Expansion of g.f. 1/(1 - 33*x + x^2).
 * https://oeis.org/A200441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33*a(n-1) - a(n-2) with a(0)=1, a(1)=33
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cnt = 0;       /* number of elements currently cached */
    static size_t capacity = 0;  /* allocated size of cache */

    /* Initialise cache with first two terms */
    if (cnt == 0) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 33;
        cnt = 2;
    }

    /* Return immediately if we already have the value */
    if ((size_t)n < cnt)
        return cache[(size_t)n];

    /* Ensure sufficient storage */
    size_t needed = (size_t)n + 1;
    if (needed > capacity) {
        capacity = needed;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* Compute missing values using the recurrence */
    for (idx_t i = cnt; i <= n; ++i) {
        cache[(size_t)i] =
            ((seq_t)33 * cache[(size_t)(i - 1)]) - cache[(size_t)(i - 2)];
    }

    cnt = needed;
    return cache[(size_t)n];
}
