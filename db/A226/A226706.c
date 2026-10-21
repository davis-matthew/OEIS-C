/*
 * A226706  G.f.: 1 / sqrt(1 + 12*x*G(x)^4 - 16*x*G(x)^5) where G(x) = 1 +
 *          x*G(x)^6 is the g.f. of A002295.
 * https://oeis.org/A226706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226706
 *
 * Definition (OEIS):
 *     a(0) = 1
 *     For n ≥ 1,
 *         a(n) = ( Σ_{k=0..n} C(3n+2k,n-k)·C(3n-2k,k)  –  Σ_{k=1..n-1} a(k)·a(n-k) ) / 2
 *     where C(m,r) is the binomial coefficient.
 *
 * The sequence satisfies the convolution identity
 *     Σ_{k=0..n} a(k)·a(n-k) = Σ_{k=0..n} C(3n+2k,n-k)·C(3n-2k,k).
 *
 * This implementation memoises values and rebuilds the cache up to the
 * requested index n.  It uses the provided binomial helper which returns
 * seq_t.  Recursion depth is O(1) and the total time is O(n²).
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;     /* cache[0..cache_cap-1] holds a(0) … a(cache_cap-1) */
static size_t cache_cap = 0;    /* number of elements currently cached          */

/* Ensure the cache contains values up to and including index N.  */
static void ensure_capacity(idx_t N)
{
    if ((size_t)N < cache_cap)
        return;                       /* already cached */

    size_t new_cap = (size_t)N + 1;    /* need indices 0 … N inclusive */

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    /* Compute new entries; existing entries are unchanged. */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        idx_t n = (idx_t)i;

        if (n == 0) {                /* a(0) = 1 */
            cache[i] = 1;
            continue;
        }

        /* Σ_{k=1..n-1} a(k)·a(n-k) */
        seq_t conv_sum = 0;
        for (idx_t k = 1; k < n; ++k)
            conv_sum += cache[k] * cache[n - k];

        /* Σ_{k=0..n} C(3n+2k,n-k)·C(3n-2k,k) */
        seq_t rhs_sum = 0;
        for (idx_t k = 0; k <= n; ++k) {
            seq_t b1 = binomial((idx_t)(3 * n + 2 * k), n - k);
            seq_t b2 = binomial((idx_t)(3 * n - 2 * k), k);
            rhs_sum += b1 * b2;
        }

        cache[i] = (rhs_sum - conv_sum) / 2;
    }

    cache_cap = new_cap;
}

extern seq_t a(idx_t n)
{
    ensure_capacity(n);
    return cache[(size_t)n];
}
