/*
 * A169317  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of reduced words of length n in a Coxeter group
 *            with 16 generators satisfying (S_i)^2 = (S_i S_j)^30 = I. It can
 *            be computed by the following finite‑state DP: Keep track of the
 *            current alternating run length (1 … 29). For n = 0, a(0) = 1;
 *            for n ≥ 1, initialise dp[1] = 16 and then it...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;          // a(0) = 1

    /* dp[l] holds the number of words of current length ending with
       alternating run length l (1 ≤ l ≤ 29). */
    seq_t dp[30] = {0}, newdp[30];
    dp[1] = (seq_t)16;             // a(1) = 16

    for (idx_t len = 2; len <= n; ++len)
    {
        /* clear newdp */
        for (idx_t i = 1; i <= 29; ++i)
            newdp[i] = (seq_t)0;

        /* from run length 1: must go to length 2 with 15 possibilities */
        newdp[2] += dp[1] * (seq_t)15;

        /* for run lengths 2 … 28 */
        for (idx_t l = 2; l <= 28; ++l)
        {
            /* reset: 14 possibilities → length 1 */
            newdp[1] += dp[l] * (seq_t)14;
            /* continue: 1 possibility → length l+1 */
            newdp[l + 1] += dp[l];
        }

        /* run length 29 → only reset allowed (11)? actually 14 possibilities */
        newdp[1] += dp[29] * (seq_t)14;

        /* copy back */
        for (idx_t i = 1; i <= 29; ++i)
            dp[i] = newdp[i];
    }

    /* sum all states to get a(n) */
    seq_t result = (seq_t)0;
    for (idx_t i = 1; i <= 29; ++i)
        result += dp[i];
    return result;
}
