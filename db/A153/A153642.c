/*
 * A153642  a(n) = 4*n^2 + 24*n + 8.
 * https://oeis.org/A153642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*n^2 + 24*n + 8
 * Range:     u8 n <= 5, u16 n <= 125, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)4 * nn * nn + (seq_t)24 * nn + (seq_t)8;
}
