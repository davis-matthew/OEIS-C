/*
 * A216108  The Wiener index of the ortho-polyphenyl chain with n hexagons
 *          (see the Dou et al. and the Deng references).
 * https://oeis.org/A216108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^3 + 36*n^2 - 21*n
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    return (seq_t)12 * nn * nn * nn + (seq_t)36 * nn * nn - (seq_t)21 * nn;
}
