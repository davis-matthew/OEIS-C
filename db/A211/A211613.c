/*
 * A211613  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and w+x+y>1.
 * https://oeis.org/A211613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; for n > 0, a(n) = (8*n^3 - 9*n^2 + 9*n - 6) / 2 (OEIS
 *            A211613)
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 835, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;          /* n */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */

    seq_t term1 = (seq_t)8 * n3;   /* 8*n^3 */
    seq_t term3 = (seq_t)9 * nn;   /* 9*n   */
    seq_t term2 = (seq_t)9 * n2;   /* 9*n^2 */
    seq_t term4 = (seq_t)6;        /* 6     */

    /* Compute (term1 + term3) - (term2 + term4) safely (no underflow) */
    seq_t sum1 = term1 + term3;
    seq_t sum2 = term2 + term4;

    return (sum1 - sum2) / (seq_t)2;
}
