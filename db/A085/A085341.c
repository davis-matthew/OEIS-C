/*
 * A085341  Number of primes between sigma(n) and n inclusive.
 * https://oeis.org/A085341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(sigma(n)) - pi(n) (Number of primes between σ(n) and n
 *            inclusive)
 * Range:     u8 n <= 225, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 97 terms through n = 9320 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Compute the sum of divisors σ(n) */
    seq_t sigma = sigma_of(n);

    /* Convert σ(n) to an idx_t for prime counting.  σ(n) is always <= n*2 for the tested range. */
    idx_t sigma_idx = (idx_t)sigma;

    /* Count primes ≤ σ(n) and ≤ n.  compute_pi returns seq_t. */
    seq_t pi_sigma = prime_pi(sigma_idx);
    seq_t pi_n     = prime_pi(n);

    /* The requested value is the difference */
    return pi_sigma - pi_n;
}
