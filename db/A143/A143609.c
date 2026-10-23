/*
 * A143609  Numerators of the upper principal and intermediate convergents to
 *          2^(1/2).
 * https://oeis.org/A143609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2)-a(n-4) for n ≥ 5 a(1) = 2, a(2) = 3, a(3) = 10,
 *            a(4) = 17 This implements A143609 (numerators of the upper
 *            principal and intermediate convergents to √2). The function
 *            memoises all computed terms so that calls are O(n) in total,
 *            independent of call order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;     /* 1‑based array of already computed terms */
static size_t cache_cap = 0;    /* capacity of cache */
static idx_t max_n = 0;         /* largest n for which cache[n] is defined */

seq_t a(idx_t n)
{
    /* undefined behavior for n < 1 – return 0 for safety */
    if (n < 1)
        return (seq_t)0;

    /* Ensure the cache has room for index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute any missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[1] = (seq_t)2; break;
            case 2: cache[2] = (seq_t)3; break;
            case 3: cache[3] = (seq_t)10; break;
            case 4: cache[4] = (seq_t)17; break;
            default: cache[i] = (seq_t)(6 * cache[i - 2] - cache[i - 4]); break;
        }
    }
    max_n = n;
    return cache[n];
}
