/*
 * A015575  Expansion of x/(1 - 8*x - 5*x^2).
 * https://oeis.org/A015575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 5*a(n-2), with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t computed = 0;          /* highest index computed so far */

    /* Make sure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first terms if this is the first call. */
    if (computed == 0) {
        cache[0] = (seq_t)0;
        if (n >= 1) {
            cache[1] = (seq_t)1;
            computed = 1;
        }
    }

    /* Compute any missing terms up to n. */
    while (computed < n) {
        size_t i = computed + 1;
        cache[i] = 8 * cache[i - 1] + 5 * cache[i - 2];
        computed = i;
    }

    return cache[n];
}
