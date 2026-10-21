/*
 * A211612  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and w+x+y>=0.
 * https://oeis.org/A211612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(8*n*n + 3*n - 3)/2
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 833, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if (nn == 0) return 0;
    seq_t inner = (seq_t)8 * nn * nn + (seq_t)3 * nn - (seq_t)3;
    return nn * inner / 2;
}
