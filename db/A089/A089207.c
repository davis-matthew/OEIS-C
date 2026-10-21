/*
 * A089207  a(n) = 4*n^3 + 2*n^2.
 * https://oeis.org/A089207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + 2*n^2
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 4 * nn * nn * nn + 2 * nn * nn;
}
