/*
 * A063488  a(n) = (2*n-1)*(n^2 -n +2)/2.
 * https://oeis.org/A063488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(n^2 -n +2)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* convert index to output type */
    seq_t term1 = (seq_t)2 * nn - (seq_t)1;   /* 2*n - 1 */
    seq_t term2 = nn * nn - nn + (seq_t)2;    /* n^2 - n + 2 */
    return term1 * term2 / (seq_t)2;          /* final division by 2 */
}
