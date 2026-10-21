/*
 * A066883  Number of primes in the interval [p(n), p(n)^2] minus p(n), where
 *          p(n) is the n-th prime.
 * https://oeis.org/A066883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = pi(p(n)^2) - pi(p(n)-1) - p(n)
 * Range:     u8 n <= 14, u16 n <= 156, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = pi(p(n)^2) - (n-1) - p(n), where p(n) is the nth prime */

#include "../utils/get_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* nth prime */
    seq_t p = (seq_t)get_prime(n);

    /* pi(p(n)^2) */
    idx_t limit = (idx_t)p * (idx_t)p;          /* p^2 fits in idx_t */
    seq_t pi_p2 = compute_pi(limit);

    /* pi(p(n)-1) = n-1  (since p(n) is the nth prime) */
    seq_t pi_p_minus1 = (seq_t)(n - 1);

    return pi_p2 - pi_p_minus1 - p;
}
