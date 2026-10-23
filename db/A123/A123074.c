/*
 * A123074  Number of ordered triples of primes (p,q,r) such that pqr = n.
 * https://oeis.org/A123074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 / (∏ e_p!) if the sum of prime exponents of n equals 3,
 *            otherwise a(n) = 0. Here e_p are the exponents in the prime
 *            factorization
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t num = (uint64_t)n;

    /* Factor n: primes[i] holds the i-th distinct prime, exps[i] its exponent. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    /* Total number of prime factors counting multiplicity. */
    uint64_t total = 0;
    for (size_t i = 0; i < cnt; ++i) total += exps[i];

    if (total != 3) return 0;

    /* Compute 6 divided by the product of factorials of the exponents. */
    seq_t res = 6;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] == 2) res /= 2;   /* divide by 2! */
        else if (exps[i] == 3) res /= 6; /* divide by 3! */
    }
    return res;
}
