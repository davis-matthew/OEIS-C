/*
 * A040024  Continued fraction for sqrt(30).
 * https://oeis.org/A040024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 5; for n >= 1, a(n) = 2 if n is odd, 10 if n is even
 * Range:     u8 n <= 9583, u16 n <= 9583, u32 n <= 9583, u64 n <= 9583,
 *            u128 n <= 9583, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 5;
    return (n & 1) ? 2 : 10;
}
