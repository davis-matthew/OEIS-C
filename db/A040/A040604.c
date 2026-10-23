/*
 * A040604  Continued fraction for sqrt(630).
 * https://oeis.org/A040604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 25 if n==0, 50 if n even but >0, 10 if n odd
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)25;
    if ((n & 1) == 0)
        return (seq_t)50;
    return (seq_t)10;
}
