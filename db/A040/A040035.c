/*
 * A040035  Continued fraction for sqrt(42).
 * https://oeis.org/A040035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 6; for n ≥ 1, a(n) = 2 if n is odd, 12 if n is even. This
 *            is the list of partial quotients of the continued fraction for
 *            sqrt(42)
 * Range:     u8 n <= 9910, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)6;
    }
    /* n is unsigned; n & 1 is 1 for odd n, 0 for even n */
    return (n & 1) ? (seq_t)2 : (seq_t)12;
}
