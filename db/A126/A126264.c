/*
 * A126264  a(n) = 5*n^2 + 3*n.
 * https://oeis.org/A126264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 + 3*n
 * Range:     u8 n <= 6, u16 n <= 114, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)5 * nn * nn + (seq_t)3 * nn;
}
