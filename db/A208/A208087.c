/*
 * A208087  Number of 6 X (n+1) 0..1 arrays with every 2 X 2 subblock having
 *          the same number of equal edges as its horizontal neighbors and a
 *          different number from its vertical neighbors, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A208087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-3) + a(n-4) with initial values: a(1) =
 *            72, a(2) = 168, a(3) = 402, a(4) = 1032 (sequence A208087)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the first four terms directly */
    if (n == 0) return 0; /* undefined for n=0, but return 0 */
    if (n == 1) return (seq_t)72;
    if (n == 2) return (seq_t)168;
    if (n == 3) return (seq_t)402;
    if (n == 4) return (seq_t)1032;

    /* Memoization array (index 0 unused) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;        /* allocated size in elements */
    static idx_t last_computed = 4;     /* last index that has been computed */

    /* Allocate cache if first use */
    if (!cache) {
        size_t init_cap = 10;                /* enough for first few values */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failed – unlikely */
        cache_cap = init_cap;
        cache[1] = (seq_t)72;
        cache[2] = (seq_t)168;
        cache[3] = (seq_t)402;
        cache[4] = (seq_t)1032;
    }

    /* Ensure capacity for index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failed – unlikely */
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already computed */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 3*a(i-3) + a(i-4) */
        cache[i] = (seq_t)3 * cache[i-1] - (seq_t)3 * cache[i-3] + cache[i-4];
    }
    last_computed = n;
    return cache[n];
}
