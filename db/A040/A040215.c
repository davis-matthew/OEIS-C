/*
 * A040215  Continued fraction for sqrt(231).
 * https://oeis.org/A040215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 15; for n > 0, a(n) = 5 if n is odd, 30 if n is even
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 15;
    return (n & 1) ? 5 : 30;
}
