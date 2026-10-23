/*
 * A167866  Length of the longest partition of n into distinct parts greater
 *          than 1, with each part divisible by the next one.
 * https://oeis.org/A167866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 0, for n >= 2, a(n) = 1 + max_{d|n, d>1}
 *            a((n-d)/d)
 * Range:     u8 n <= 8545, u16 n <= 8545, u32 n <= 8545, u64 n <= 8545,
 *            u128 n <= 8545, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function is defined with external linkage as required. */
static seq_t *cache = NULL;      /* Dynamic cache of computed values. */
static idx_t cache_len = 0;      /* Current size of the cache. */

seq_t a(idx_t n)
{
    /* Extend the cache if necessary. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;   /* Allocation failed – return 0 as a graceful fallback. */
        cache = new_cache;

        /* Compute the requested values (and any intermediate ones). */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 1) {
                cache[i] = 0;
                continue;
            }
            seq_t best = 0;
            /* Loop over divisors d > 1 of i.  For each divisor we consider both
               d and its paired divisor i/d.  Each contributes an index to look
               up in the cache that is strictly less than i, guaranteeing that the
               needed value is already known. */
            for (idx_t d = 2; d * d <= i; ++d) {
                if (i % d != 0) continue;

                /* Divisor d > 1 */
                idx_t idx1 = i / d - 1;          /* (i-d)/d = i/d - 1 = (i/d) - 1 */
                seq_t val1 = cache[idx1];
                if (val1 > best) best = val1;

                /* Paired divisor other = i/d > 1 */
                idx_t other = i / d;
                if (other > 1 && other != d) {
                    idx_t idx2 = d - 1;        /* (i - other) / other = d - 1 */
                    seq_t val2 = cache[idx2];
                    if (val2 > best) best = val2;
                }
            }
            /* The divisor d = i itself gives a value a(0) = 0; it cannot improve
               the maximum, so it is effectively ignored. */
            cache[i] = best + 1;
        }
        cache_len = new_len;
    }
    return cache[n];
}
