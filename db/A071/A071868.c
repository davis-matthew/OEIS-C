/*
 * A071868  Number of integers k (1 <= k <= n) such that k^2+1 is prime.
 * https://oeis.org/A071868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = # { k | 1 ≤ k ≤ n and k^2 + 1 is prime }
 * Range:     u8 n <= 15, u16 n <= 418, u32 n <= 418, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Ignore zero or negative indices – the OEIS definition starts at 1. */
    if (n == 0) return 0;

    /* Persistent cache of prefix counts: cache[i] holds a(i+1). */
    static seq_t *cache = NULL;        /* cache[0] = a(1) etc. */
    static idx_t cached_n = 0;         /* largest n already cached */

    /* Return cached result if we already have it. */
    if (n <= cached_n) return cache[n - 1];

    /* Resize cache to hold up to n; use malloc/realloc so it works
       in both C and C++. */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!new_cache) return 0;          /* allocation failure – give up */
    cache = new_cache;

    /* Current accumulator equals the last cached value, or 0 if nothing
       has been cached yet. */
    seq_t acc = (cached_n == 0) ? 0 : cache[cached_n - 1];

    /* Compute new terms for k = cached_n+1 … n. */
    for (idx_t k = cached_n + 1; k <= n; ++k)
    {
        /* Compute k^2 + 1 using seq_t to avoid overflow. */
        seq_t val = (seq_t)k * (seq_t)k + (seq_t)1;

        /* Count it if k^2 + 1 is prime. */
        if (is_prime_big(val)) ++acc;

        cache[k - 1] = acc;
    }

    cached_n = n;
    return cache[n - 1];
}
