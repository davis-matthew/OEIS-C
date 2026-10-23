/*
 * A185568  Number of (n+1)X2 0..3 arrays with every 2X2 subblock sum equal to
 *          some horizontal or vertical neighbor 2X2 subblock sum.
 * https://oeis.org/A185568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×2 arrays of digits 0…3 such that every
 *            2×2 subblock sum equals one of its horizontal or vertical
 *            neighbours. Computed by dynamic programming over row sums
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: `seq_t` and `idx_t` are provided by the build environment.  
   The implementation uses only integer arithmetic and standard
   operators, so it is valid in both C11 and C++17. */
seq_t a(idx_t n)
{
    /* If n == 0, the definition is not used – return 0. */
    if (n == 0) return (seq_t)0;

    /* Number of row patterns for each possible row sum (0–6). */
    const int F[7] = {1, 2, 3, 4, 3, 2, 1};

    /* DP arrays: dp[row_sum][subblock_sum][singleton_flag] */
    /* singleton_flag: 1 -> current run length is 1 (cannot change yet)  */
/* 0 -> current run length >= 2 (allowed to change) */
    static seq_t dp[7][13][2];
    static seq_t nxt[7][13][2];
    /* Initialise dp to 0 */
    for (int t = 0; t < 7; ++t)
        for (int s = 0; s < 13; ++s)
            for (int f = 0; f < 2; ++f)
                dp[t][s][f] = (seq_t)0;

    /* Initial step: choose first two rows t0 and t1 */
    for (int t0 = 0; t0 < 7; ++t0)
        for (int t1 = 0; t1 < 7; ++t1)
        {
            int s0 = t0 + t1;                 /* sum of first 2×2 block */
            seq_t w = (seq_t)F[t0] * (seq_t)F[t1];
            dp[t1][s0][1] += w;               /* run length is 1 (singleton) */
        }

    /* Process remaining subblocks (positions 1 to n-1) */
    for (idx_t step = 1; step < n; ++step)
    {
        /* Clear next array */
        for (int t = 0; t < 7; ++t)
            for (int s = 0; s < 13; ++s)
                for (int f = 0; f < 2; ++f)
                    nxt[t][s][f] = (seq_t)0;

        /* Transition */
        for (int cur = 0; cur < 7; ++cur)
            for (int lastS = 0; lastS < 13; ++lastS)
                for (int flag = 0; flag < 2; ++flag)
                {
                    seq_t curval = dp[cur][lastS][flag];
                    if (curval == (seq_t)0) continue;
                    for (int nxtrow = 0; nxtrow < 7; ++nxtrow)
                    {
                        int newS = cur + nxtrow;
                        if (newS > 12) continue;          /* sums are 0..12 */
                        int newFlag;
                        if (newS == lastS)
                            newFlag = 0;                  /* run length >= 2 */
                        else
                        {
                            if (flag == 1) continue;       /* previous run too short */
                            newFlag = 1;                  /* start new run */
                        }
                        seq_t add = curval * (seq_t)F[nxtrow];
                        nxt[nxtrow][newS][newFlag] += add;
                    }
                }
        /* Move next to dp for next iteration */
        for (int t = 0; t < 7; ++t)
            for (int s = 0; s < 13; ++s)
                for (int f = 0; f < 2; ++f)
                    dp[t][s][f] = nxt[t][s][f];
    }

    /* Sum all states where the final run has length >= 2 */
    seq_t result = (seq_t)0;
    for (int t = 0; t < 7; ++t)
        for (int s = 0; s < 13; ++s)
            result += dp[t][s][0];   /* flag 0 -> run length >= 2 */

    return result;
}
