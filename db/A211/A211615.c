/*
 * A211615  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and -1<=w+x+y<=1.
 * https://oeis.org/A211615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6-9*n+9*n*n for n>0; a(0)=0
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    return 9 * nn * nn - 9 * nn + 6;
}
