/*
 * A040757  Continued fraction for sqrt(786).
 * https://oeis.org/A040757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 28; for n > 0, a(n) = 28 if n is odd, otherwise a(n) = 56
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;
    return (n & 1u) ? (seq_t)28 : (seq_t)56;
}
