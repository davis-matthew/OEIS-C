/*
 * A090345  Number of Motzkin paths of length n with no level steps at even
 *          level.
 * https://oeis.org/A090345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Σ_{i=0}^{n-2} a(i)*a(n-2-i) with a(0) = 1, a(1)
 *            = 0
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 53, u128 n <= 101,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following helper ensures we have space to store 0..n. */
#include "../utils/ensure_cache.h"   /* provides ensure_cache(idx_t, seq_t**, size_t*) */

static seq_t *cache = NULL;          /* array holding the sequence values */
static size_t cache_cap = 0;         /* current allocated capacity (in elements) */
static idx_t cache_filled = 0;       /* number of pre‑computed elements */

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    /* Make sure the cache can hold indices [0..n] */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise base values if they are not yet present */
    if (cache_filled == 0)    { cache[0] = 1; cache_filled = 1; }
    if (cache_filled == 1)    { cache[1] = 0; cache_filled = 2; }

    /* If we already know a(n), return it immediately */
    if (cache_filled > n)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t m = cache_filled; m <= n; ++m) {
        seq_t sum = 0;
        /* The convolution sum Σ_{i=0}^{m-2} a(i)*a(m-2-i) */
        for (idx_t i = 0; i <= m - 2; ++i) {
            sum += cache[i] * cache[m - 2 - i];
        }
        cache[m] = cache[m - 1] + sum;
    }

    cache_filled = n + 1;
    return cache[n];
}
