/*
 * A052149  Number of nonsquare rectangles on an n X n board.
 * https://oeis.org/A052149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n*(n-1)*(n+1)*(3*n+2)/12
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 194, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type first */
    seq_t nn = (seq_t)n;

    /* multiply all factors using seq_t arithmetic */
    seq_t prod = nn;
    prod *= (nn - 1);                /* (n-1)          */
    prod *= (nn + 1);                /* (n+1)          */
    prod *= ((seq_t)3 * nn + (seq_t)2);  /* 3*n+2          */
    prod /= (seq_t)12;               /* divide by 12    */

    return prod;
}
