/*
 * A040554  Continued fraction for sqrt(579).
 * https://oeis.org/A040554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 24; for n ≥ 1, a(n) = 16 if n is odd, otherwise 48
 *            (period 2)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)24;
    return (n & 1) ? (seq_t)16 : (seq_t)48;
}
