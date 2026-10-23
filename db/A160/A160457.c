/*
 * A160457  a(n) = n^2 - 2*n + 2.
 * https://oeis.org/A160457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - 2*n + 2
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * nn + (seq_t)2) - (seq_t)2 * nn;
}
