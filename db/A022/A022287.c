/*
 * A022287  a(n) = n*(29*n + 1)/2.
 * https://oeis.org/A022287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(29*n + 1)/2
 * Range:     u8 n <= 4, u16 n <= 67, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (sn * (sn * 29 + 1)) / 2;
}
