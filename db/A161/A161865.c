/*
 * A161865  Numerators of ratio of nonprimes in a square interval to that of
 *          nonprimes in that interval and its successor.
 * https://oeis.org/A161865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the reduced fraction * (N((n+1)²) – N(n²)) /
 *            (N((n+2)²) – N(n²)) * where N(x) = x – π(x) and π(x) is the
 *            prime‑count function. * The fraction is reduced by dividing
 *            numerator and denominator by * their greatest common divisor;
 *            only the numerator is returned
 * Range:     u8 n <= 13, u16 n <= 302, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute the relevant squares as seq_t to avoid overflow in small
     * seq_t types; for large seq_t they fit comfortably. */
    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t n1_sq  = (seq_t)(n + 1) * (seq_t)(n + 1);
    seq_t n2_sq  = (seq_t)(n + 2) * (seq_t)(n + 2);

    /* Prime counts using the provided π function. */
    seq_t pi_n   = prime_pi((idx_t)n_sq);
    seq_t pi_n1  = prime_pi((idx_t)n1_sq);
    seq_t pi_n2  = prime_pi((idx_t)n2_sq);

    /* Number of non‑primes up to each square. */
    seq_t N_n    = n_sq   - pi_n;
    seq_t N_n1   = n1_sq  - pi_n1;
    seq_t N_n2   = n2_sq  - pi_n2;

    /* Numerator and denominator of the fraction. */
    seq_t numer  = N_n1 - N_n;
    seq_t denom  = N_n2 - N_n;

    /* Reduce the fraction by dividing by the gcd. */
    seq_t g = gcd(numer, denom);
    numer /= g;          /* denom/g is ignored; only the reduced numerator is returned */

    return numer;
}
