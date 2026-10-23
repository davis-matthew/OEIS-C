/*
 * A040707  Continued fraction for sqrt(735).
 * https://oeis.org/A040707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 27 if n = 0, 9 if n >= 1 and n is odd, 54 if n >= 1 and n
 *            is even
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 27;
    }
    return (n & 1) ? 9 : 54;
}
