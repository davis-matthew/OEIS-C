/*
 * A127193  A 9th-order Fibonacci sequence.
 * https://oeis.org/A127193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) ... a(9) = 1, a(10) = 9, for n ≥ 11: a(n) = 2*a(n-1) –
 *            a(n-10)
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 39, u64 n <= 71, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recursive 9th-order Fibonacci sequence (A127193). */
seq_t a(idx_t n)
{
    /* Static cache for memoisation.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size (includes index 0) */
    static idx_t cached_n = 0;    /* highest index computed */

    /* Initialise cache with the first 10 terms. */
    if (!cache) {
        /* Allocate space for indices 0..10 (11 elements). */
        cache = (seq_t *)malloc((size_t)(11) * sizeof(seq_t));
        for (idx_t i = 1; i <= 9; ++i) cache[i] = (seq_t)1;
        cache[10] = (seq_t)9;
        cache_cap = 11;
        cached_n = 10;
    }

    /* Grow cache if necessary. */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 1; i <= cached_n; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing terms up to n. */
    if (n > cached_n) {
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = (seq_t)1;
            } else if (i == 10) {
                cache[i] = (seq_t)9;
            } else {
                /* a(i) = 2*a(i-1) - a(i-10) */
                cache[i] = (seq_t)(2 * cache[i - 1] - cache[i - 10]);
            }
        }
        cached_n = n;
    }

    return cache[n];
}
