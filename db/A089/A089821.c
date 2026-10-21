/*
 * A089821  Number of subsets of {1,.., n} containing exactly one prime.
 * https://oeis.org/A089821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) * 2^(n - pi(n)) (pi = A000720)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   compute_pi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Count primes ≤ n */
    seq_t pi = compute_pi(n);

    /* Compute exponent n - pi(n) */
    idx_t exp = n - (idx_t)pi;

    /* 2^(n - pi(n)) */
    seq_t pow = pow2(exp);

    /* Multiply and return */
    return pi * pow;
}
