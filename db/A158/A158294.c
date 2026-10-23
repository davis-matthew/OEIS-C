/*
 * A158294  Averages of squares of divisors of numbers in A020486 (i.e.,
 *          numbers m with property that average of squares of divisors is an
 *          integer: sigma_0(m) divides sigma_2(m)).
 * https://oeis.org/A158294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001157(A020486(n)) / A000005(A020486(n)) where A020486
 *            is the list of integers m for which sigma_2(m) / tau(m) is an
 *            integer. A001157 is the sum of squares of divisors (sigma_2) and
 *            A000005 is the divisor count (tau). This implementation builds
 *            the sequence on demand by testing successive...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 504, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

static seq_t *cache_a = NULL;            /* cached a(n) values */
static size_t cache_len = 0;             /* number of cached values */
static size_t cache_cap = 0;             /* capacity of cache_a */
static uint64_t next_m = 1;              /* next integer to test for A020486 */

/* resize the cache to at least 'new_cap' capacity */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap : 4;
    while (cap < new_cap) cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache_a, cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache_a = new_arr;
    cache_cap = cap;
}

/* compute a single term of the sequence.  The function keeps
   a static cache so subsequent calls are inexpensive. */
seq_t a(idx_t n)
{
    if (n == 0) abort();               /* offset is 1, so n>=1 required */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t m = next_m++;
        /* factor m and compute tau and sigma_2 in one pass */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(m, primes, exps, &cnt);

        /* compute tau = product (exps[i] + 1)   */
        seq_t tau = 1;
        for (size_t i = 0; i < cnt; ++i) {
            tau *= (seq_t)(exps[i] + 1);
        }

        /* compute sigma_2 = product [ (p^(2*e+2)-1)/(p^2-1) ] */
        seq_t sigma2 = 1;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            idx_t exp = (idx_t)exps[i];

            /* numerator: p^(2*exp+2) - 1 */
            seq_t num = pow_int(p, 2 * exp + 2) - 1;

            /* denominator: p^2 - 1 */
            seq_t den = pow_int(p, 2) - 1;

            /* term = num / den (exact division) */
            sigma2 *= (num / den);
        }

        /* handle m == 1: cnt == 0 => sigma2 remains 1, tau remains 1 */

        /* keep only numbers where sigma2 is divisible by tau */
        if (sigma2 % tau == 0) {
            seq_t val = sigma2 / tau;
            ensure_cache_capacity(cache_len + 1);
            cache_a[cache_len++] = val;
        }
    }

    return cache_a[n - 1];
}
