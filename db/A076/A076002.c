/*
 * A076002  Seventh column of triangle A075502.
 * https://oeis.org/A076002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A076002 
    Formula: a(n) = (7^n) * S2(n+7, 7)      (S2 = Stirling numbers of the second kind)
*/

seq_t a(idx_t n)
{
    /* 7^n  */
    seq_t pow7 = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow7 *= 7;

    /* Stirling numbers S2(n+7, 7)  */
    seq_t S[8] = {0};
    S[0] = 1;                      /* S(0,0)=1 */

    for (idx_t i = 1; i <= n + 7; ++i)
    {
        for (int k = 7; k > 0; --k)
            S[k] = (seq_t)k * S[k] + S[k - 1];
        S[0] = 0;                  /* for n>0, S(n,0)=0 */
    }

    return pow7 * S[7];
}
