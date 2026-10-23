/*
 * A040236  Continued fraction for sqrt(252).
 * https://oeis.org/A040236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=15; for n>0, a(n)=1 if n is odd, a(n)=6 if n≡2 (mod 4), and
 *            a(n)=30 if n≡0 (mod 4). This follows from the multiplicative
 *            definition with a(2)=6 and a(2^e)=30 for e≥2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)15;
    if ((n & 1) != 0)   /* odd n */
        return (seq_t)1;

    /* even n: determine exponent of 2 */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }

    return (e == 1) ? (seq_t)6 : (seq_t)30;
}
