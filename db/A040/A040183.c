/*
 * A040183  Continued fraction for sqrt(198).
 * https://oeis.org/A040183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 14 if n == 0 or n odd; a(n) = 28 if n >= 2 and even
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)14;
    if ((n & 1) == 1) return (seq_t)14;
    return (seq_t)28;
}
