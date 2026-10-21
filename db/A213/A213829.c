/*
 * A213829  Principal diagonal of the convolution array A213828.
 * https://oeis.org/A213829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(3*n^3 - 3*n^2 + n)
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = 3 * nn * nn * nn - 3 * nn * nn + nn;
    return 2 * temp;
}
