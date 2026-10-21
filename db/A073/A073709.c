/*
 * A073709  First differences of A073708.
 * https://oeis.org/A073709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   Recurrence derived from the generating function A(x) =
 *            A(x^2)^2/(1-x). Initial value: a(0) = 1. For n ≥ 1: if n is odd,
 *            a(n) = a(n-1); if n is even, a(n) = a(n-1) + Σ_{i=0}^{n/2}
 *            a(i)*a(n/2-i). The sequence therefore satisfies a(2k)=a(2k+1)
 *            and is computed incrementally with memoization for effic...
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 95, u64 n <= 301,
 *            u128 n <= 1041, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Ensure that values up to index n are computed and cached. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;

    idx_t old_size = cache_size;
    idx_t new_len   = n + 1;

    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
    if (!new_cache)
        abort();

    /* Copy already computed values if any. */
    if (cache) {
        for (idx_t i = 0; i < old_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache     = new_cache;
    cache_size = new_len;

    /* Compute the remaining values. */
    for (idx_t idx = old_size; idx < new_len; ++idx) {
        if (idx == 0) {
            cache[0] = 1;
        } else if (idx & 1) { /* odd index */
            cache[idx] = cache[idx - 1];
        } else { /* even index */
            seq_t sum = 0;
            idx_t k = idx / 2;
            for (idx_t i = 0; i <= k; ++i)
                sum += cache[i] * cache[k - i];
            cache[idx] = cache[idx - 1] + sum;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
