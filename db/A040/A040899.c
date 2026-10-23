/*
 * A040899  Continued fraction for sqrt(930).
 * https://oeis.org/A040899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 30; for n > 0: a(n) = 2 if n is odd; a(n) = 60 if n is
 *            even
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)30;
    if (n & 1) return (seq_t)2;
    return (seq_t)60;
}
