/*
 * A185572  Number of (n+1)X2 0..3 arrays with no 2X2 subblock sum equal to
 *          any horizontal or vertical neighbor 2X2 subblock sum.
 * https://oeis.org/A185572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{w_0…w_n, w_i≠w_{i+2}} ∏_{i=0}^n c_{w_i} where c =
 *            {1,2,3,4,3,2,1}. This counts (n+1)×2 arrays with entries 0..3
 *            such that no two consecutive 2×2 subblocks have equal sums
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Number of possible row sums (0‑6). */
    enum { SUMS = 7 };

    /* Multiplicities of each row sum. */
    static const seq_t weight[SUMS] = {
        (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4,
        (seq_t)3, (seq_t)2, (seq_t)1
    };

    /* Sum of all multiplicities: 16. */
    const seq_t sum_weights =
        weight[0] + weight[1] + weight[2] + weight[3] +
        weight[4] + weight[5] + weight[6];

    /* Base case: two rows (n = 1). */
    if (n == 1) {
        return sum_weights * sum_weights;
    }

    /* DP tables for weighted counts of sequences ending with
       (prev, prevprev).  dp[prev][prevprev] holds the total weight
       for the current prefix. */
    seq_t dp[SUMS][SUMS];
    seq_t next[SUMS][SUMS];

    /* Initialize for the first two rows. */
    for (int a = 0; a < SUMS; ++a) {
        for (int b = 0; b < SUMS; ++b) {
            dp[a][b] = weight[a] * weight[b];
        }
    }

    /* Number of additional rows beyond the first two. */
    idx_t steps = n - 1;   /* (n+1) rows → (n+1 - 2) transitions */

    for (idx_t step = 0; step < steps; ++step) {
        /* Zero the next table. */
        for (int i = 0; i < SUMS; ++i) {
            for (int j = 0; j < SUMS; ++j) {
                next[i][j] = (seq_t)0;
            }
        }

        for (int prev = 0; prev < SUMS; ++prev) {
            for (int prevprev = 0; prevprev < SUMS; ++prevprev) {
                seq_t cur = dp[prev][prevprev];
                if (cur == (seq_t)0) continue;

                for (int s = 0; s < SUMS; ++s) {
                    if (s == prevprev) continue;           /* w_i ≠ w_{i+2} */
                    next[s][prev] = next[s][prev] + cur * weight[s];
                }
            }
        }

        /* Move next into dp for the next iteration. */
        for (int i = 0; i < SUMS; ++i) {
            for (int j = 0; j < SUMS; ++j) {
                dp[i][j] = next[i][j];
            }
        }
    }

    /* Sum all possibilities after the last row. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < SUMS; ++i) {
        for (int j = 0; j < SUMS; ++j) {
            result = result + dp[i][j];
        }
    }
    return result;
}
