/*
 * A101986  Maximum sum of products of successive pairs in a permutation of
 *          order n+1.
 * https://oeis.org/A101986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 9*n + 1)/6
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn;            /* n^2          */
    t = t * 2 + nn * 9 + 1;        /* 2*n^2 + 9*n + 1 */
    return nn * t / 6;             /* n * ( ... ) / 6 */
}
