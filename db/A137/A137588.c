/*
 * A137588  Number of primes not greater than n that are greater than the
 *          number of primes not greater than n.
 * https://oeis.org/A137588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{p prime: pi(n)<p<=n} = pi(n)-pi(pi(n)) where pi is
 *            A000720
 * Range:     u8 n <= 1727, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset starts at 1; behavior below is irrelevant */
    seq_t pi_n     = prime_pi(n);         /* number of primes ≤ n */
    seq_t pi_pi_n  = prime_pi((idx_t)pi_n); /* number of primes ≤ pi(n) */
    return pi_n - pi_pi_n;               /* primes in (pi(n), n] */
}
