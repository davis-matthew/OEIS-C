/*
 * A005997  Number of paraffins.
 * https://oeis.org/A005997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n^2-(-1)^n+1)/8
 * Range:     u8 n <= 9, u16 n <= 63, u32 n <= 1551, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t term = 2 * m * m - (n & 1 ? -1 : 1) + 1;
    return (m + 1) * term / 8;
}
