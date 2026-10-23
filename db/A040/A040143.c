/*
 * A040143  Continued fraction for sqrt(156).
 * https://oeis.org/A040143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 12; for n > 0, a(n) = 2 if n is odd, 24 if n is even
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
    return (seq_t)(n & 1 ? 2 : 24);
}
