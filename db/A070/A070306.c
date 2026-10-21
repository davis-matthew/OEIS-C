/*
 * A070306  a(n) = 2*phi(n)/2^omega(n).
 * https://oeis.org/A070306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*phi(n)/2^omega(n) (omega = number of distinct prime
 *            factors of n)
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, count_distinct_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/pow2.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* number of distinct prime factors of n */
    idx_t distinct = (idx_t)count_distinct_primes((uint64_t)n);

    /* φ(n) */
    seq_t phi_n = calc_phi((seq_t)n);

    /* denominator: 2^omega */
    seq_t denom = pow2(distinct);

    /* 2*φ(n)/2^omega */
    return (seq_t)2 * phi_n / denom;
}
