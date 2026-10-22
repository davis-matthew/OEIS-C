/*
 * A000202  a(8i+j) = 13i + a(j), where 1<=j<=8.
 * https://oeis.org/A000202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((13*n - 1)/8)
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = (n - 1) / 8;
    idx_t r = (n - 1) % 8;
    return (seq_t)q * 13 + (seq_t)((13 * r + 12) / 8);
}
