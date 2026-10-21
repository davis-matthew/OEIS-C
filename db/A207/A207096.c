/*
 * A207096  Number of 0..4 arrays x(0..n-1) of n elements with each no smaller
 *          than the sum of its two previous neighbors modulo 5.
 * https://oeis.org/A207096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5, a(2)=15.  
   For n≥3 let dp[a][b] be the number of length‑len sequences ending with
   values a,b (0 ≤ a,b<5).  When extending the sequence, a new value c must
   satisfy c ≥ (a+b) mod 5.  Update rule:
       dp'[b][c] += dp[a][b]  for all c ≥ (a+b)%5.
   After n‑2 updates, a(n) = Σ_{a,b} dp[a][b}. */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* out of range, not used */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)15;

    seq_t dp[5][5] = {0};
    seq_t next[5][5];

    /* initialise for length 2: b >= a */
    for (idx_t a_idx = 0; a_idx < 5; ++a_idx) {
        for (idx_t b_idx = 0; b_idx < 5; ++b_idx) {
            if (b_idx >= a_idx) dp[a_idx][b_idx] = (seq_t)1;
        }
    }

    for (idx_t len = 3; len <= n; ++len) {
        /* zero next array */
        for (idx_t i = 0; i < 5; ++i)
            for (idx_t j = 0; j < 5; ++j)
                next[i][j] = (seq_t)0;

        /* transition */
        for (idx_t a = 0; a < 5; ++a) {
            for (idx_t b = 0; b < 5; ++b) {
                seq_t val = dp[a][b];
                if (val == (seq_t)0) continue;
                idx_t t = (a + b) % 5;
                for (idx_t c = t; c < 5; ++c) {
                    next[b][c] += val;
                }
            }
        }

        /* copy back */
        for (idx_t i = 0; i < 5; ++i)
            for (idx_t j = 0; j < 5; ++j)
                dp[i][j] = next[i][j];
    }

    /* sum all counts for length n */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < 5; ++i)
        for (idx_t j = 0; j < 5; ++j)
            sum += dp[i][j];

    return sum;
}
