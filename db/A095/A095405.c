/*
 * A095405  Numbers n such that Sum-of-digits-of-n =
 *          Sum-of-digits-of-all-distinct-prime-factors-of-n.
 * https://oeis.org/A095405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A095405 – numbers for which the sum of digits equals the sum of
   the digit sums of their distinct prime factors.
   This implementation builds the sequence incrementally and caches
   the results.  It uses the provided utilities digit_sum and factor_u64.
   The function works for any seq_t and idx_t supplied by the build
   environment. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/factor_u64.h"

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The largest integer that has been tested so far */
    static uint64_t last_checked = 0;

    /* This implementation does not care about n = 0; return 0. */
    if (n == 0) return (seq_t)0;

    /* If we already have enough terms, return fast. */
    if (n <= (idx_t)cache_len)
        return cache[n - 1];

    /* Temporary arrays for factorisation (max 20 distinct primes for 64‑bit). */
    uint64_t primes[20], exps[20];
    size_t cnt;

    /* Continue testing integers until we have n terms. */
    while (cache_len < (size_t)n) {
        ++last_checked;                     /* candidate integer */

        seq_t sum_n = digit_sum((seq_t)last_checked);

        /* Get distinct prime factors of last_checked. */
        factor_u64(last_checked, primes, exps, &cnt);

        seq_t sum_pf = 0;
        for (size_t i = 0; i < cnt; ++i)
            sum_pf += digit_sum((seq_t)primes[i]);

        if (sum_n == sum_pf) {
            /* Grow cache if necessary. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                while (new_cap < cache_len + 1)
                    new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* unrecoverable memory error */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
