/*
 * A211480  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w + 2x + 3y = 1.
 * https://oeis.org/A211480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n+1)^2/3)
 * Range:     u8 n <= 13, u16 n <= 221, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 2 + 1;   /* 2*n + 1 as a seq_t */
    return (t * t) / 3;           /* integer division truncates toward 0 */
}
