/*
 * A205591  a(1) = 1, a(n) = a(floor((2n-1)/3)) + a(floor(2n/3)) for n > 1.
 * https://oeis.org/A205591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1) = 1; a(n) = a(floor((2n-1)/3)) + a(floor(2n/3)) for n > 1 */
#include <stddef.h>
#include "../utils/ensure_cache.h"

/* static cache for memoized values; 0 means uncomputed (all terms are positive) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Recursive computation with memoisation */
seq_t a(idx_t n)
{
    if (n == 1)          /* base case */
        return 1;

    /* make sure the cache can hold index n */
    ensure_cache(n, &cache, &cache_cap);

    /* return already computed value if present */
    if (cache[n] != 0)
        return cache[n];

    /* compute recursively */
    idx_t k1 = (2*n - 1) / 3;
    idx_t k2 = (2*n)     / 3;
    seq_t v1 = a(k1);
    seq_t v2 = a(k2);
    seq_t result = v1 + v2;

    /* store for future calls */
    cache[n] = result;
    return result;
}
