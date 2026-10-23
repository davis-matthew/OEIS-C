/*
 * A172202  Number of ways to place 3 nonattacking kings on a 3 X n board.
 * https://oeis.org/A172202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(9*n^2 - 45*n + 70)/2 for n>=2, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 985, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t nn   = n;                       /* convert index to seq_t */
    seq_t inner = (seq_t)9 * nn * nn      /* 9*n^2  */
                 - (seq_t)45 * nn          /* -45*n  */
                 + 70;                    /* +70    */
    seq_t prod  = (nn - 2) * inner;       /* (n-2)*inner */
    return prod / 2;                      /* divide by 2 */
}
