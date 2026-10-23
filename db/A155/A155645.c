/*
 * A155645  a(n) = 7^n+6^n-1.
 * https://oeis.org/A155645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n + 6^n - 1 Recurrence used: a(0) = 1, a(1) = 12, a(n)
 *            = 13*a(n-1) - 42*a(n-2) - 30 (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

extern seq_t a(idx_t n) {
    /* Handle the trivial base case first */
    if (n == 0) {
        return 1;
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;   /* highest index computed so far */

    /* Ensure that the cache has enough space for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first two terms */
    if (max_computed == 0) {
        cache[0] = 1;
        cache[1] = 12;
        max_computed = 1;
    }

    /* If the requested term is already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute missing terms using the recurrence relation */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val *= 13;
        seq_t t = cache[i - 2];
        t *= 42;
        val -= t;
        val -= 30;
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
