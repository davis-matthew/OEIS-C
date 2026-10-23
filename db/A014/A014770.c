/*
 * A014770  Squares of even pentagonal numbers.
 * https://oeis.org/A014770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( (m*(3*m - 1)/2) )^2, where m = 2*n + (n & 1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)n << 1) + (n & 1);
    seq_t pent = m * (3*m - 1) / 2;
    return pent * pent;
}
