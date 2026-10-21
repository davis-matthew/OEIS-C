/*
 * A068032  Number of subsets of {1,2,3,...,n} that sum to 0 mod 11.
 * https://oeis.org/A068032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of subsets of {1,2,…,n} whose sum is ≡ 0 (mod 11)
 */
#define MOD 11

seq_t a(idx_t n)
{
    seq_t dp[MOD];
    for (int r = 0; r < MOD; ++r) dp[r] = 0;
    dp[0] = 1;                       /* empty subset */

    for (idx_t i = 1; i <= n; ++i) {
        int v = (int)(i % (idx_t)MOD);        /* i mod 11 */

        seq_t newdp[MOD];
        for (int r = 0; r < MOD; ++r)        /* start with subsets that don't use i */
            newdp[r] = dp[r];

        for (int r = 0; r < MOD; ++r) {      /* add subsets that include i */
            int nr = r + v;
            if (nr >= MOD) nr -= MOD;
            newdp[nr] += dp[r];
        }

        for (int r = 0; r < MOD; ++r)        /* copy back for next iteration */
            dp[r] = newdp[r];
    }

    return dp[0];         /* subsets whose sum ≡ 0 (mod 11) */
}
