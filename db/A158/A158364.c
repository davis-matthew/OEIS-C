/*
 * A158364  a(n) = 529*n^2 - 2*n.
 * https://oeis.org/A158364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 529*n^2 - 2*n
 * Range:     u8 none, u16 n <= 11, u32 n <= 2849, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to the sequence type */
    return 529 * nn * nn - 2 * nn;
}
