/*
 * A165806  a(n) = 15n^2 + 3n + 1.
 * https://oeis.org/A165806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n^2 + 3*n + 1
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * (sn * 15 + 3) + 1;
}
