/*
 * A059512  For n>=2, the number of (s(0), s(1), ..., s(n-1)) such that 0 <
 *          s(i) < 5 and |s(i) - s(i-1)| <= 1 for i = 1,2,....,n-1, s(0) = 2,
 *          s(n-1) = 2.
 * https://oeis.org/A059512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A059512 – number of sequences (s0, …, s(n‑1)) with  
   1 ≤ si ≤ 4,  s0 = sn‑1 = 2,  |si – s(i‑1)| ≤ 1
   Computed by dynamic programming on the 4‑node path graph.
   For n = 0 the sequence is defined as 0.
*/

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* dp[v] = number of ways to reach value v at current position */
    seq_t dp[5]  = {0};
    seq_t ndp[5] = {0};

    dp[2] = 1;             /* start at value 2 */

    /* perform n‑1 transitions */
    for (idx_t step = 1; step < n; ++step)
    {
        for (int v = 1; v <= 4; ++v)
        {
            seq_t sum = 0;
            if (v > 1) sum += dp[v - 1];   /* from v‑1 */
            sum += dp[v];                  /* from v   */
            if (v < 4) sum += dp[v + 1];   /* from v+1 */
            ndp[v] = sum;
        }
        /* copy back for next step */
        for (int v = 1; v <= 4; ++v)
            dp[v] = ndp[v];
    }

    return dp[2];   /* end must be 2 */
}
