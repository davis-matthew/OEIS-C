/*
 * A211618  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and 2w+x+y>1.
 * https://oeis.org/A211618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A211618: Number of ordered triples (w,x,y) with 2w+x+y>1.
   Formula:  a(0) = 0
             a(n) = 4*n^3 - 3*n^2 + 3*n - 2   (n>0 even)
             a(n) = 4*n^3 - 3*n^2 + 3*n - 1   (n>0 odd)  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          /* n^2 */
    seq_t cube = sq * nn;        /* n^3 */
    seq_t term1 = (seq_t)4 * cube;   /* 4*n^3 */
    seq_t term2 = (seq_t)3 * sq;     /* 3*n^2 */
    seq_t term3 = (seq_t)3 * nn;     /* 3*n */

    seq_t result = term1 - term2 + term3;
    result -= ((n & 1) == 0) ? ((seq_t)2) : ((seq_t)1);
    return result;
}
