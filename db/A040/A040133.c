/*
 * A040133  Continued fraction for sqrt(146).
 * https://oeis.org/A040133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=12, a(n)=12 if n is odd, a(n)=24 if n is even and n>=2
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)12;
    return ((n & 1) != 0) ? (seq_t)12 : (seq_t)24;
}
