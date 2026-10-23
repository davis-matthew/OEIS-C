/*
 * A040211  Continued fraction for sqrt(227).
 * https://oeis.org/A040211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 15 if n==0 or n is odd, otherwise 30 (continued fraction
 *            of sqrt(227))
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)15;
    return (n & 1) ? (seq_t)15 : (seq_t)30;
}
