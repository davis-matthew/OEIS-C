/*
 * A040874  Continued fraction for sqrt(905).
 * https://oeis.org/A040874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 30, a(n) = 12 if n is odd, 60 if n is even (n > 0)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)30;
    return (n & 1) ? (seq_t)12 : (seq_t)60;
}
