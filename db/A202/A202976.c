/*
 * A202976  Number of nX5 0..1 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A202976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by a DP over 5‑bit row states (dynamic‑programming
 *            recurrence)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool validity[32][32][32];
static bool validity_initialized = false;

/* Precompute the validity table: whether the middle row 'curr' satisfies the
   neighbor condition when its up neighbor is 'prev' and its down neighbor is 'next'. */
static void precompute_validity(void)
{
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            for (int next = 0; next < 32; ++next) {
                bool ok = true;
                for (int j = 0; j < 5 && ok; ++j) {
                    if (curr & (1 << j)) {
                        int neigh = 0;
                        if (j > 0 && (curr & (1 << (j - 1)))) neigh++;
                        if (j < 4 && (curr & (1 << (j + 1)))) neigh++;
                        if (prev & (1 << j)) neigh++;
                        if (next & (1 << j)) neigh++;
                        if (neigh < 2) ok = false;
                    }
                }
                validity[prev][curr][next] = ok;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (!validity_initialized) {
        precompute_validity();
        validity_initialized = true;
    }

    /* dp[prev][curr]  -- number of ways to reach a configuration where the
       last two rows are 'prev' and 'curr', with all constraints satisfied
       for the rows up to 'curr' except the last row. */
    static seq_t dp[32][32];
    static seq_t dp_next[32][32];
    /* initialize to zeros */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* first row: previous is all zeros */
    for (int r = 0; r < 32; ++r)
        dp[0][r] = 1;

    /* Add rows 2..n */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0) continue;
                for (int next = 0; next < 32; ++next) {
                    if (validity[prev][curr][next]) {
                        dp_next[curr][next] += val;
                    }
                }
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* final row must satisfy condition with down neighbor zero */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (validity[prev][curr][0]) {
                ans += dp[prev][curr];
            }
        }
    }
    return ans;
}
