/*
 * A056578  a(n) = 1 + 2*n + 3*n^2 + 4*n^3.
 * https://oeis.org/A056578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2*n + 3*n^2 + 4*n^3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return (seq_t)1 + (seq_t)2 * nn + (seq_t)3 * n2 + (seq_t)4 * n3;
}
