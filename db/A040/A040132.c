/*
 * A040132  Continued fraction for sqrt(145).
 * https://oeis.org/A040132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 12, a(n) = 24 for n ≥ 1
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)12 : (seq_t)24;
}
