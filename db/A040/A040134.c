/*
 * A040134  Continued fraction for sqrt(147).
 * https://oeis.org/A040134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=12; a(n)=8 if n is odd, 24 if n is even (n>=1)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)12;
    return (n & 1) ? (seq_t)8 : (seq_t)24;
}
