/*
 * A161893  Denominators of S(n) = Sum_{j=2..n} (pi((j+1)^2) -
 *          pi(j^2))/(pi((j+1)^2)*pi(j^2)) where pi(k) = A000720(k).
 * https://oeis.org/A161893
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, less
 * Formula:   a(n) = denominator of the reduced fraction S(n) = 1/2 -
 *            1/pi((n+1)^2) where pi(k) = number of primes ≤ k. Since S(n) =
 *            1/2 - 1/P with P = pi((n+1)^2), the fraction is (P-2)/(2P); the
 *            denominator is (2P)/gcd(P-2,2P)
 * Range:     u8 n <= 25, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"        /* gcd(seq_t a, seq_t b)           */
#include "../utils/prime_pi.h"   /* seq_t prime_pi(idx_t limit)     */

seq_t a(idx_t n)
{
    /* For indices below the offset behaviour is irrelevant. */
    if (n < 2) return (seq_t)1;

    /* Compute P = pi((n+1)^2). */
    idx_t sq = (idx_t)(n + 1) * (idx_t)(n + 1);
    seq_t P  = prime_pi(sq);

    /* Numerator and denominator of the fraction (P-2)/(2P). */
    seq_t num = P - (seq_t)2;
    seq_t den = P * (seq_t)2;

    /* Reduce the fraction and return the denominator. */
    seq_t g = gcd(num, den);
    if (g > (seq_t)1) den /= g;

    return den;
}
