/*
 * A068013  Number of subsets of {1,2,3,...,n} that sum to 0 mod 7.
 * https://oeis.org/A068013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,n} whose sum is ≡ 0 (mod 7)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed by a 7‑state dynamic programming over the subset sums. */
seq_t a(idx_t n)
{
    /* dp[r] = number of subsets of {1..i} with sum ≡ r (mod 7) */
    seq_t dp[7] = {0};
    dp[0] = 1;                     /* the empty subset */
    seq_t next[7];

    for (idx_t i = 1; i <= n; ++i)
    {
        int imod = (int)(i % 7);
        for (int r = 0; r < 7; ++r)
        {
            int src = r - imod;
            if (src < 0) src += 7;
            next[r] = dp[r] + dp[src];
        }
        for (int r = 0; r < 7; ++r)
            dp[r] = next[r];
    }
    return dp[0];
}
