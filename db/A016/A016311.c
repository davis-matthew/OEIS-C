/*
 * A016311  Expansion of 1/((1-2*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A016311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 56*a(n-2) + 2^n for n ≥ 2 with a(0)=1,
 *            a(1)=17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ensure_cache.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* base cases */
    if (n == 0) return 1;
    if (n == 1) return 17;

    /* make sure we have enough space for all indices up to n */
    if (cache == NULL || cache_cap <= (size_t)n)
        ensure_cache(n, &cache, &cache_cap);

    /* initialise the first two values (they will be overwritten on later calls,
       but this guarantees a known starting point) */
    cache[0] = 1;
    cache[1] = 17;

    /* Compute the recurrence up to n, overwriting any previously stored
       values so we need not keep a separate "computed" flag. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = 15 * cache[i - 1];
        seq_t term2 = 56 * cache[i - 2];
        seq_t term3 = pow2(i);          /* 2^i */
        cache[i] = term1 + term3 - term2;
    }

    return cache[n];
}
