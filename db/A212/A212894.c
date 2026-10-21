/*
 * A212894  Number of (w,x,y,z) with all terms in {0,...,n} and (least
 *          gapsize)=1.
 * https://oeis.org/A212894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)*(3*n^2-3*n+5) for n>1 a(0)=0, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 895, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    seq_t m = (seq_t)n;
    /* term2 = 3*m*m - 3*m + 5 */
    seq_t term2 = 3 * m * m - 3 * m + (seq_t)5;
    return 2 * (m - (seq_t)1) * term2;
}
