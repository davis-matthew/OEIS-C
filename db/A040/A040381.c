/*
 * A040381  Continued fraction for sqrt(402).
 * https://oeis.org/A040381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 20; a(n) = 20 if n is odd, 40 if n is even and n > 0
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || (n & 1))
        return 20;
    return 40;
}
