/*
 * A082053  Sum of divisors of n that are not of the form 4k+3.
 * https://oeis.org/A082053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, d%4!=3} d (sum of all divisors of n minus sum
 *            of the divisors that are 3 mod 4)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   enumerate_divisors_and_sum3, factoring_and_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/enumerate_divisors_and_sum3.h"
#include "../utils/factoring_and_sigma.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by definition of the sequence. */
    uint64_t primes[128];
    int exps[128];
    int cnt;
    seq_t sigma;

    factoring_and_sigma(n, primes, exps, &cnt, &sigma);
    seq_t sum3;
    enumerate_divisors_and_sum3(primes, exps, cnt, &sum3);
    return sigma - sum3;
}
