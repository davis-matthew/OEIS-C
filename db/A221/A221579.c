/*
 * A221579  A sum over partitions (q=10), see first comment.
 * https://oeis.org/A221579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all partitions of n of ∏ over distinct part
 *            sizes s of f(m_s) where f(m) = 10^(m-1) * 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of conjugacy classes in GL(n,10) */
seq_t a(idx_t n)
{
    /* Allocate two working arrays of size n+1 (one for current dp,
       one for the updated dp after processing each part size).  */
    seq_t *dp, *tmp;
#ifdef __cplusplus
    dp  = new seq_t[n + 1];
    tmp = new seq_t[n + 1];
#else
    dp  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    tmp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    /* Initialize dp[0]=1, rest 0 */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;

    /* Main DP: for each possible part size i (1..n) */
    for (idx_t i = 1; i <= n; ++i) {
        /* Copy current dp into tmp to start accumulation */
        for (idx_t j = 0; j <= n; ++j) {
            tmp[j] = dp[j];
        }

        /* Current weight f(m) starts at m = 1 : 9,
           and each subsequent multiplicity multiplies by 10 */
        seq_t cur = 9;
        for (idx_t m = 1; ; ++m) {
            idx_t shift = i * m;
            if (shift > n)
                break;

            /* Add contributions for multiplicity m */
            for (idx_t t = 0; t + shift <= n; ++t) {
                tmp[t + shift] += dp[t] * cur;
            }
            /* Prepare next weight f(m+1) = 10 * f(m) */
            cur *= 10;
        }

        /* Swap dp and tmp for next iteration */
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
    }

    /* Result is dp[n] */
    seq_t result = dp[n];

    /* Free allocated memory */
#ifdef __cplusplus
    delete[] dp;
    delete[] tmp;
#else
    free(dp);
    free(tmp);
#endif

    return result;
}
