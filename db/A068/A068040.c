/*
 * A068040  Number of subsets of {1,2,3,...,n} that sum to 0 mod 19.
 * https://oeis.org/A068040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #subsets of {1,2,…,n} whose sum ≡ 0 (mod 19)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: dp[r] = number of subsets with sum ≡ r (mod 19).  Start with dp[0] = 1 (empty set).  
   For each i = 1…n: newdp[r] = dp[r] + dp[(r‑i) mod 19].  After all i, a(n) = dp[0]. */

seq_t a(idx_t n)
{
    seq_t dp[19];
    for (int r = 0; r < 19; ++r)
        dp[r] = 0;
    dp[0] = 1;                           /* empty set */

    for (idx_t i = 1; i <= n; ++i) {
        int imod = (int)(i % 19);        /* i modulo 19, fits in int */

        seq_t newdp[19];
        for (int r = 0; r < 19; ++r) {
            int prev = r - imod;
            if (prev < 0)
                prev += 19;
            newdp[r] = dp[r] + dp[prev];
        }
        for (int r = 0; r < 19; ++r)
            dp[r] = newdp[r];
    }

    return dp[0];
}
