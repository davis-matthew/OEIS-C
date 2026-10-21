/*
 * A231705  Number of (n+1)X(3+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors
 *          equal to one.
 * https://oeis.org/A231705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming with a 3‑row sliding window over
 *            a (n+1) x 4 binary grid. For each cell the condition
 *            sum_of_neighbors * 2 <= number_of_neighbors is checked, where
 *            neighbors are horizontal, vertical and antidiagonal ones. The DP
 *            keeps the two previous rows; after adding a new row...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_COLS 4
#define STATE_CNT 17          /* 0..15 are real rows, 16 is “missing” sentinel */

static inline bool row_valid(idx_t prevprev, idx_t prev, idx_t next)
{
    /* Check the “prev” row (pattern stored in ‘prev’) using its
       neighboring rows ‘prevprev’ (above) and ‘next’ (below). */
    for (idx_t j = 0; j < MAX_COLS; ++j) {
        int neigh_cnt = 0;
        int neigh_sum = 0;

        /* left neighbor in the same row */
        if (j > 0) {
            ++neigh_cnt;
            neigh_sum += (prev >> (j - 1)) & 1U;
        }
        /* right neighbor in the same row */
        if (j + 1 < MAX_COLS) {
            ++neigh_cnt;
            neigh_sum += (prev >> (j + 1)) & 1U;
        }
        /* up neighbor */
        if (prevprev < STATE_CNT) {
            ++neigh_cnt;
            neigh_sum += (prevprev >> j) & 1U;
        }
        /* down neighbor */
        if (next < STATE_CNT) {
            ++neigh_cnt;
            neigh_sum += (next >> j) & 1U;
        }
        /* up‑right (antidiagonal) neighbor */
        if (prevprev < STATE_CNT && j + 1 < MAX_COLS) {
            ++neigh_cnt;
            neigh_sum += (prevprev >> (j + 1)) & 1U;
        }
        /* down‑left (antidiagonal) neighbor */
        if (next < STATE_CNT && j > 0) {
            ++neigh_cnt;
            neigh_sum += (next >> (j - 1)) & 1U;
        }

        /* strict majority forbidden: neigh_sum * 2 <= neigh_cnt */
        if ((neigh_sum << 1) > neigh_cnt)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for the offset, but safe */

    const idx_t sentinel = STATE_CNT;   /* 16 */
    static seq_t dp[STATE_CNT + 1][STATE_CNT + 1];
    static seq_t newdp[STATE_CNT + 1][STATE_CNT + 1];

    /* initialise dp to zero */
    for (idx_t i = 0; i <= sentinel; ++i)
        for (idx_t j = 0; j <= sentinel; ++j)
            dp[i][j] = 0;

    dp[sentinel][sentinel] = 1;   /* no rows processed yet */

    /* The grid has (n+1) rows indexed 0..n */
    for (idx_t step = 0; step <= n; ++step) {
        /* reset newdp */
        for (idx_t i = 0; i <= sentinel; ++i)
            for (idx_t j = 0; j <= sentinel; ++j)
                newdp[i][j] = 0;

        for (idx_t prevprev = 0; prevprev <= sentinel; ++prevprev) {
            for (idx_t prev = 0; prev <= sentinel; ++prev) {
                seq_t ways = dp[prevprev][prev];
                if (ways == 0)
                    continue;

                for (idx_t curr = 0; curr <= 15; ++curr) {
                    /* If 'prev' row is real, check its validity with the
                       window (prevprev, prev, curr).  Skip check when
                       prev is the sentinel (missing). */
                    if (prev < sentinel && !row_valid(prevprev, prev, curr))
                        continue;

                    /* Accumulate ways for the new state (prev, curr) */
                    newdp[prev][curr] += ways;
                }
            }
        }

        /* swap dp and newdp for next iteration */
        for (idx_t i = 0; i <= sentinel; ++i)
            for (idx_t j = 0; j <= sentinel; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* After processing all rows, validate the last row (prev) with no
       row below (sentinel). */
    seq_t result = 0;
    for (idx_t prevprev = 0; prevprev <= sentinel; ++prevprev) {
        for (idx_t prev = 0; prev <= sentinel; ++prev) {
            seq_t ways = dp[prevprev][prev];
            if (ways == 0)
                continue;
            if (prev < sentinel && row_valid(prevprev, prev, sentinel))
                result += ways;
        }
    }
    return result;
}
