/*
 * A195587  a(n) = A163659(n^2), where A163659 is the logarithmic derivative
 *          of Stern's diatomic series (A002487).
 * https://oeis.org/A195587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 255, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A195587: a(n) = (2^(2*e2+1)-1) * (e3>0 ? -2 : 1)
   where e2 = exponent of 2 in n, e3 = exponent of 3 in n.
   This uses the multiplicativity of A163659 and the fact that
   A163659(2^k) = 2^(k+1)-1 for even k, and A163659(3^k) = -2 for k>=1.
   For all other primes p, A163659(p^k) = 1.
*/

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe fallback */

    idx_t t = n;
    /* exponent of 2 in n */
    idx_t e2 = 0;
    while ((t & 1) == 0) { t >>= 1; ++e2; }

    /* exponent of 3 in n */
    idx_t e3 = 0;
    while (t % 3 == 0) { t /= 3; ++e3; }

    seq_t result = 1;

    if (e2 > 0) {
        /* compute 2^(2*e2+1)-1 safely; e2 <= 63 for any n that fits in idx_t */
        seq_t pow2 = ((seq_t)1) << (2*e2 + 1);
        result *= (pow2 - 1);
    }

    if (e3 > 0) {
        result *= -2;
    }

    return result;
}
