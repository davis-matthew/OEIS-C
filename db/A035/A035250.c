/*
 * A035250  Number of primes between n and 2n (inclusive).
 * https://oeis.org/A035250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(2*n) - pi(n-1) (number of primes in [n,2n] inclusive)
 * Range:     u8 n <= 2049, u16 n <= 9327, u32 n <= 9327, u64 n <= 9327,
 *            u128 n <= 9327, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset; for n==0 the behavior is irrelevant. */
    idx_t two_n = n << 1;                    /* 2*n */
    seq_t pi_2n = prime_pi(two_n);           /* primes <= 2*n   */
    seq_t pi_n_1 = prime_pi(n - 1);          /* primes <= n-1   */
    return pi_2n - pi_n_1;                   /* primes in [n,2n] */
}
