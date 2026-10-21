/*
 * A068035  Number of subsets of {1,2,3,...,n} that sum to 0 mod 14.
 * https://oeis.org/A068035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,n} whose sum is 0 (mod 14)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* DP over residues modulo 14 */
    seq_t dp[14];
    seq_t next[14];

    /* initialise dp[0] = 1 (empty subset), others 0 */
    for (int r = 0; r < 14; ++r)
        dp[r] = 0;
    dp[0] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        int imod = (int)(i % 14);          /* i modulo 14 */

        /* copy current dp to next */
        for (int r = 0; r < 14; ++r)
            next[r] = dp[r];

        /* add contributions of subsets that include element i */
        for (int r = 0; r < 14; ++r) {
            int src = (r - imod + 14) % 14;
            next[r] = next[r] + dp[src];
        }

        /* swap next into dp */
        for (int r = 0; r < 14; ++r)
            dp[r] = next[r];
    }

    return dp[0];
}
