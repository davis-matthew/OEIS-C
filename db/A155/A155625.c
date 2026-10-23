/*
 * A155625  a(n) = 11^n + 4^n - 1.
 * https://oeis.org/A155625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + 4^n - 1 Recurrence: a(0) = 1, a(1) = 14, a(n) =
 *            15*a(n-1) - 44*a(n-2) - 30 for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* Special cases that fit trivially */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;

    /* Cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t last_index = 0;   /* index of the last computed value (>=1 after init) */
    static size_t cap = 0;         /* allocated capacity of cache */

    /* Ensure the cache can hold indices up to n */
    ensure_cache(n + 1, &cache, &cap);

    /* Initialise the first two terms once */
    if (last_index < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        last_index = 1;
    }

    /* Compute new values up to n */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        seq_t term = cache[i - 1] * (seq_t)15;
        term -= cache[i - 2] * (seq_t)44;
        term -= (seq_t)30;
        cache[i] = term;
    }
    last_index = n;

    return cache[n];
}
