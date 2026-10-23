/*
 * A158557  a(n) = 225*n^2 + 15.
 * https://oeis.org/A158557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 225*n^2 + 15
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 4369, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type first */
    seq_t nn = (seq_t)n;
    /* compute 225*n^2 + 15 using integer arithmetic only */
    return (seq_t)225 * nn * nn + (seq_t)15;
}
