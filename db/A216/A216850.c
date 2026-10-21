/*
 * A216850  Number of distinct infinite sets of primes congruent to a subset
 *          of 1..n mod n.
 * https://oeis.org/A216850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^phi(n) - 1) * 2^omega(n)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   calc_phi, count_distinct_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the helper functions provided in the
 * utility library: calc_phi for Euler's totient, count_distinct_primes
 * for the number of distinct prime factors, and pow2 for powers of two.
 */

#include "../utils/calc_phi.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Euler's totient of n */
    seq_t phi = calc_phi((seq_t)n);

    /* Number of distinct prime factors of n */
    int  omega_int = count_distinct_primes((uint64_t)n);
    seq_t omega = (seq_t)omega_int;

    /* Powers of two */
    seq_t two_to_phi   = pow2((idx_t)phi);
    seq_t two_to_omega = pow2((idx_t)omega);

    /* (2^phi - 1) * 2^omega */
    return (two_to_phi - (seq_t)1) * two_to_omega;
}
