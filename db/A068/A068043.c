/*
 * A068043  Number of subsets of {1,2,3,...,n} that sum to 0 mod 25.
 * https://oeis.org/A068043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,...,n} whose sum ≡ 0 (mod 25).
 *            Computes this by dynamic programming over residues mod 25
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t dp[25];
    seq_t next[25];
    /* initialise dp */
    for (int r = 0; r < 25; ++r) dp[r] = 0;
    dp[0] = 1;                           /* empty set */

    for (idx_t i = 1; i <= n; ++i) {
        int m = (int)(i % 25);            /* current element modulo 25 */

        /* copy current counts to next */
        for (int r = 0; r < 25; ++r) next[r] = dp[r];

        /* add subsets with the new element */
        for (int r = 0; r < 25; ++r) {
            int idx = (r + m) % 25;
            next[idx] += dp[r];
        }

        /* move next to dp for next iteration */
        for (int r = 0; r < 25; ++r) dp[r] = next[r];
    }

    return dp[0];                         /* subsets summing to 0 mod 25 */
}
