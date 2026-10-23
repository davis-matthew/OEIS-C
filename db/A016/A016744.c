/*
 * A016744  a(n) = (2*n)^4.
 * https://oeis.org/A016744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^4
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^4 as (n*n)*(n*n) and then multiply by 16. */
    seq_t nn   = (seq_t)n;          /* convert index to seq_t */
    seq_t nn2  = nn * nn;           /* n^2 */
    seq_t nn4  = nn2 * nn2;         /* n^4 */
    return (seq_t)16 * nn4;         /* 16 * n^4 = (2*n)^4 */
}
