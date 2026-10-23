/*
 * A152811  a(n) = 2*(n^2 + 2*n - 2).
 * https://oeis.org/A152811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2 * ( n^2 + 2*n - 2 )
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence element type first */
    seq_t nn = (seq_t)n;
    /* Compute using only integer arithmetic */
    seq_t val = 2 * (nn * nn + 2 * nn - 2);
    return val;
}
