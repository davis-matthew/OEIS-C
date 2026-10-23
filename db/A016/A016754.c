/*
 * A016754  Odd squares: a(n) = (2n+1)^2. Also centered octagonal numbers.
 * https://oeis.org/A016754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^2 (equivalently 4*n*(n+1)+1)
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast idx_t to seq_t before arithmetic to avoid overflow of idx_t */
    seq_t m = (seq_t)n;          /* convert index to sequence type */
    seq_t t = 2 * m + 1;         /* compute 2*n + 1 */
    return t * t;                /* square the result */
}
