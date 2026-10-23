/*
 * A157362  a(n) = 49*n^2 - 2*n.
 * https://oeis.org/A157362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*n^2 - 2*n
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9362, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n to seq_t before any computation */
    seq_t nn = (seq_t)n;
    return (seq_t)49 * nn * nn - (seq_t)2 * nn;
}
