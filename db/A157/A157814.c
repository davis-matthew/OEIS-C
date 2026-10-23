/*
 * A157814  a(n) = 27225*n^2 - 2*n.
 * https://oeis.org/A157814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27225*n^2 - 2*n
 * Range:     u8 none, u16 n <= 1, u32 n <= 397, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)27225 * nn * nn - (seq_t)2 * nn;
}
