/*
 * A155127  a(n) = 6*a(n-1) + 6*a(n-2), n>2, a(0)=1, a(1)=5, a(2)=35.
 * https://oeis.org/A155127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 6*a(n-2), a(0)=1, a(1)=5, a(2)=35
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t max_cached = (idx_t)-1;

    /* Initialize cache with the first three terms */
    if (max_cached == (idx_t)-1) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)35;
        max_cached = 2;
    }

    /* Return cached value if already computed */
    if (n <= max_cached) return cache[n];

    /* Expand cache up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) { /* realloc failed, return last known value */ return cache[n]; }
    cache = tmp;

    /* Compute missing terms iteratively */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = (seq_t)6 * cache[i - 1] + (seq_t)6 * cache[i - 2];
    }

    max_cached = n;
    return cache[n];
}
