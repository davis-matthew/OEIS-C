/*
 * A095403  Sum of digits of n minus the sum of digits of all distinct prime
 *          factors of n.
 * https://oeis.org/A095403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign, changed
 * Formula:   a(n) = digit_sum(n) - Σ digit_sum(p) for all distinct prime
 *            factors p of n
 * Range:     i8 n <= 702, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_sum_local, factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor.h"
#include "../utils/digit_sum_local.h"

seq_t a(idx_t n)
{
    /* Sum of digits of n */
    seq_t sum_n = digit_sum_local((seq_t)n);

    /* Factorize n and accumulate digit sums of distinct prime factors */
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor((uint64_t)n, primes, exps, &cnt);

    seq_t sum_pf = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum_pf += digit_sum_local((seq_t)primes[i]);
    }

    /* Difference */
    return sum_n - sum_pf;
}
