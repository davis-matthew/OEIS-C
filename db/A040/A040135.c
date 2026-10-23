/*
 * A040135  Continued fraction for sqrt(148).
 * https://oeis.org/A040135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 12; a(n) = 6 for odd n; a(n) = 24 for even n > 0
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)12;
    return ((n & (idx_t)1) != 0) ? (seq_t)6 : (seq_t)24;
}
