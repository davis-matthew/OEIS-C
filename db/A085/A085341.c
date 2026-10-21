/*
 * A085341  Number of primes between sigma(n) and n inclusive.
 * https://oeis.org/A085341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(sigma(n)) - pi(n) (Number of primes between σ(n) and n
 *            inclusive)
 * Range:     u8 n <= 225, u16 n <= 9320, u32 n <= 9320, u64 n <= 9320,
 *            u128 n <= 9320, big any n
 * Helpers:   compute_pi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Compute the sum of divisors σ(n) */
    seq_t sigma = sigma_of(n);

    /* Convert σ(n) to an idx_t for prime counting.  σ(n) is always <= n*2 for the tested range. */
    idx_t sigma_idx = (idx_t)sigma;

    /* Count primes ≤ σ(n) and ≤ n.  compute_pi returns seq_t. */
    seq_t pi_sigma = compute_pi(sigma_idx);
    seq_t pi_n     = compute_pi(n);

    /* The requested value is the difference */
    return pi_sigma - pi_n;
}
