/*
 * A123908  Number of sequences with terms 1, 2 or 3 summing to n with no
 *          three consecutive 1's.
 * https://oeis.org/A123908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + 2*a(n-4) + a(n-5) Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 41, u64 n <= 81, u128 n <= 161,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure the cache can hold all indices up to n. */
    if (cap <= n) {
        ensure_cache(n, &cache, &cap);
    }

    /* Base cases (set regardless of previous content). */
    if (n >= 0) cache[0] = (seq_t)1;
    if (n >= 1) cache[1] = (seq_t)1;
    if (n >= 2) cache[2] = (seq_t)2;
    if (n >= 3) cache[3] = (seq_t)3;
    if (n >= 4) cache[4] = (seq_t)6;

    /* Compute remaining terms using the recurrence. */
    for (idx_t i = (idx_t)5; i <= n; ++i) {
        cache[i] = cache[i-2]
                   + (cache[i-3] << 1)
                   + (cache[i-4] << 1)
                   + cache[i-5];
    }

    return cache[n];
}
