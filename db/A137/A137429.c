/*
 * A137429  a(n) = -2*a(n-1) - 2*a(n-2), with a(0)=1 and a(1)=-4.
 * https://oeis.org/A137429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - 2*a(n-2), a(0) = 1, a(1) = -4
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc */

static seq_t *cache = NULL;  /* memoization buffer */
static idx_t cached_len = 0; /* number of stored terms */

/* Compute a(n) */
seq_t a(idx_t n) {
    /* Ensure the cache is initialized with the first two terms. */
    if (!cache) {
        size_t init_cap = 2;
        cache = (seq_t *)realloc(NULL, init_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-4;
        cached_len = init_cap;
    }

    /* If the requested term is already in cache, return it. */
    if (n < cached_len) {
        return cache[n];
    }

    /* Grow the cache to accommodate n. */
    if (cached_len <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cached_len = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = -2*a(i-1) - 2*a(i-2) */
        seq_t temp = (seq_t)2 * cache[i - 1] + (seq_t)2 * cache[i - 2];
        cache[i] = -temp;
    }

    return cache[n];
}
