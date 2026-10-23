/*
 * A184103  a(n) = n-1+ceiling(n^2/16); complement of A184102.
 * https://oeis.org/A184103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1+ceiling(n^2/16)
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    seq_t ceil_term = (sq + 15) / 16;
    return (nn - 1) + ceil_term;
}
