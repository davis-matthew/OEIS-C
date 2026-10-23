/*
 * A040553  Continued fraction for sqrt(578).
 * https://oeis.org/A040553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=24, a(1)=24; for n>=2, a(n)=48 if n is even, otherwise 24
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 24;          /* a(0) and a(1) */
    return (n & 1) ? 24 : 48;       /* a(n): 48 for even n, 24 for odd n */
}
