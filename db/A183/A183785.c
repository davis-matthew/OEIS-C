/*
 * A183785  Half the number of (n+1) X 3 0..2 arrays with no 2 X 2 subblock
 *          having sum 4.
 * https://oeis.org/A183785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (# of 0..2 (n+1)×3 arrays with no 2×2 subblock sum 4) / 2.
 * The number is computed by dynamic programming over 3‑digit rows.
 */
 
seq_t a(idx_t n)
{
    /* Precompute transitions between 3‑digit rows over {0,1,2}. */
    static int digit[27][3];
    static int next_cnt[27];
    static int next_list[27][27];
    static int initialized = 0;

    if (!initialized) {
        /* Row representation: index = a*9 + b*3 + c, a,b,c ∈ {0,1,2} */
        for (int idx = 0; idx < 27; ++idx) {
            int a = idx / 9;
            int b = (idx / 3) % 3;
            int c = idx % 3;
            digit[idx][0] = a;
            digit[idx][1] = b;
            digit[idx][2] = c;
        }

        for (int i = 0; i < 27; ++i) {
            for (int j = 0; j < 27; ++j) {
                int sum1 = digit[i][0] + digit[i][1] + digit[j][0] + digit[j][1];
                int sum2 = digit[i][1] + digit[i][2] + digit[j][1] + digit[j][2];
                if (sum1 != 4 && sum2 != 4) {
                    next_list[i][next_cnt[i]++] = j;
                }
            }
        }
        initialized = 1;
    }

    /* Dynamic programming over rows. */
    static seq_t dp[27];
    static seq_t ndp[27];
    for (int i = 0; i < 27; ++i) dp[i] = 1;   /* length = 1 */

    /* Perform n transitions to reach length n+1. */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 27; ++i) ndp[i] = 0;

        for (int i = 0; i < 27; ++i) {
            seq_t val = dp[i];
            if (val == 0) continue;
            int cnt = next_cnt[i];
            for (int k = 0; k < cnt; ++k) {
                int j = next_list[i][k];
                ndp[j] += val;
            }
        }

        for (int i = 0; i < 27; ++i) dp[i] = ndp[i];
    }

    /* Sum all sequences of length n+1 and divide by 2. */
    seq_t total = 0;
    for (int i = 0; i < 27; ++i) total += dp[i];
    return total / 2;
}
