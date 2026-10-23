/*
 * A167180  a(n) = pi(n) plus the number of nonprimes less than prime(n).
 * https://oeis.org/A167180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) + (prime(n)-1 - pi(prime(n)-1))
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided utilities compute_pi and get_prime. */
#include <stdint.h>
#include "../utils/compute_pi.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1; values below are undefined */

    /* nth prime */
    seq_t p = (seq_t)(uint32_t)get_prime(n);

    /* primes ≤ n */
    seq_t pi_n = compute_pi(n);

    /* primes ≤ p-1 */
    seq_t pi_p_minus1 = compute_pi((idx_t)(p - 1));

    /* a(n) = pi(n) + (p - 1 - pi(p - 1)) */
    seq_t result = pi_n - pi_p_minus1 + p - 1;
    return result;
}
