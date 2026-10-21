/*
 * A081441  a(n) = (2*n^3 - n^2 - n + 2)/2.
 * https://oeis.org/A081441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 - n^2 - n + 2)/2
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t numerator = (seq_t)2 * sn * sn * sn - sn * sn - sn + 2;
    return numerator / 2;
}
