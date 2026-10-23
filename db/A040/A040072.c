/*
 * A040072  Continued fraction for sqrt(82).
 * https://oeis.org/A040072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 9, a(n) = 18 for n >= 1
 * Range:     u8 n <= 9351, u16 n <= 9351, u32 n <= 9351, u64 n <= 9351,
 *            u128 n <= 9351, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)9 : (seq_t)18;
}
