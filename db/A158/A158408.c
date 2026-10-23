/*
 * A158408  a(n) = 900*n^2 - 2*n.
 * https://oeis.org/A158408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 900*n^2 - 2*n
 * Range:     u8 none, u16 n <= 8, u32 n <= 2184, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* Convert index to seq_t type */
    return (seq_t)900 * nn * nn   /* 900 * n^2  */
           - (seq_t)2   * nn;     /*  - 2 * n   */
}
