/*
 * A063892  a(1) = a(2) = a(3) = 1, a(n) = a(n-a(n-2))+a(n-a(n-3)) for n>3.
 * https://oeis.org/A063892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(1) = a(2) = a(3) = 1; a(n) = a(n-a(n-2)) + a(n-a(n-3)) for n>3
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Guard against non‑positive indices */
    if (n == 0) return (seq_t)0;

    /* Ensure space for the initial known values (indices 1..3) */
    if (cache == NULL) {
        /* Allocate at least 4 slots (index 0 unused, indices 1–3 used) */
        ensure_cache(4, &cache, &cache_cap);
        cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* Ensure the array is large enough for index n */
    if (n >= cache_cap) {
        ensure_cache(n + 1, &cache, &cache_cap);  /* n+1 to include index n */
    }

    /* Return already computed value if present */
    if (cache[n] != (seq_t)0) {
        return cache[n];
    }

    /* Iteratively compute all values up to n (avoids deep recursion) */
    for (idx_t i = 4; i <= n; ++i) {
        /* Skip if already known */
        if (cache[i] != (seq_t)0) {
            continue;
        }
        idx_t idx1 = i - (idx_t)cache[i - 2];
        idx_t idx2 = i - (idx_t)cache[i - 3];

        seq_t val1 = (idx1 > 0) ? cache[idx1] : (seq_t)0;
        seq_t val2 = (idx2 > 0) ? cache[idx2] : (seq_t)0;

        cache[i] = val1 + val2;
    }

    return cache[n];
}
