/*
 * A040008  Continued fraction for sqrt(12).
 * https://oeis.org/A040008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 3, a(n) = 2 if n is odd, 6 if n is even (n ≥ 1)
 * Range:     u8 n <= 9805, u16 n <= 9805, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    if (n & 1U)
        return (seq_t)2;
    return (seq_t)6;
}
