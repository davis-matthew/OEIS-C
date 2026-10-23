/*
 * A025883  Expansion of 1/((1-x^5)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A025883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2+21*n)/630 + ((4*n^2+4*n+2) mod 5)/5 + ((n^2+6)
 *            mod 7)/7)
 * Range:     u8 none, u16 n <= 464, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic without overflow (small builds) */
    seq_t sn = (seq_t)n;

    /* First term numerator: n^2 + 21n */
    seq_t N1 = sn * sn + (seq_t)21 * sn;

    /* Remainder for second term (mod 5) */
    seq_t r1 = (4 * sn * sn + 4 * sn + 2) % (seq_t)5;

    /* Remainder for third term (mod 7) */
    seq_t r2 = (sn * sn + 6) % (seq_t)7;

    /* Compute full numerator with common denominator 630:
       N = (n^2+21n) + r1*126 + r2*90  */
    seq_t N = N1 + r1 * 126 + r2 * 90;

    /* Result is floor(N/630) */
    return N / (seq_t)630;
}
