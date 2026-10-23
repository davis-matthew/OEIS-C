/*
 * A040139  Continued fraction for sqrt(152).
 * https://oeis.org/A040139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 12; a(2k+1) = 3; a(2k) = 24 for k>0
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)12;
    return (n & 1) ? (seq_t)3 : (seq_t)24;
}
