/*
 * A067707  a(n) = 3*n^2 + 12*n.
 * https://oeis.org/A067707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 12*n
 * Range:     u8 n <= 7, u16 n <= 145, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* convert index to type used for values */
    return 3 * nn * nn + 12 * nn;     /* compute 3*n^2 + 12*n */
}
