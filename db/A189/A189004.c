/*
 * A189004  Number of domino tilings of the 7 X n grid with upper left corner
 *          removed iff n is odd.
 * https://oeis.org/A189004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A189004: Number of domino tilings of the 7×n grid with the upper left
    corner removed iff n is odd.
    Computed using a column‑by‑column DP over 7-bit masks.
    States: 0…127 representing occupancy of the current column coming
    from the previous column.  The transition matrix (depends on whether
    the first column has a missing cell) is pre‑computed once.
    Complexity: O(n·128²) per call (≈160 000 operations for n=10000). */
#include <stdint.h>
#include <stdbool.h>

/* transition matrices: [missing][from][to] */
static int trans[2][128][128];
static bool trans_ready = false;

/* Recursively generate all completions of a column. */
static void dfs_gen(unsigned row, uint8_t cur, uint8_t miss,
                    uint8_t nxt, int *cnt)
{
    if (row >= 7) {
        cnt[nxt]++;            /* finished this column */
        return;
    }
    uint8_t bit = 1U << row;
    if (cur & bit || miss & bit) {
        dfs_gen(row + 1, cur, miss, nxt, cnt);
    } else {
        /* vertical domino */
        if (row + 1 < 7) {
            uint8_t bit2 = 1U << (row + 1);
            if (!(cur & bit2) && !(miss & bit2))
                dfs_gen(row + 2, cur, miss, nxt, cnt);
        }
        /* horizontal domino to next column */
        dfs_gen(row + 1, cur, miss, nxt | bit, cnt);
    }
}

/* Pre‑compute the two transition matrices. */
static void init_trans(void)
{
    if (trans_ready) return;
    for (int cur = 0; cur < 128; ++cur) {
        int cnt[128] = {0};
        dfs_gen(0, (uint8_t)cur, 0, 0, cnt);
        for (int nxt = 0; nxt < 128; ++nxt)
            trans[0][cur][nxt] = cnt[nxt];
    }
    for (int cur = 0; cur < 128; ++cur) {
        int cnt[128] = {0};
        dfs_gen(0, (uint8_t)cur, 1, 0, cnt);   /* missing upper‑left */
        for (int nxt = 0; nxt < 128; ++nxt)
            trans[1][cur][nxt] = cnt[nxt];
    }
    trans_ready = true;
}

seq_t a(idx_t n)
{
    init_trans();

    /* DP arrays: dp[curr] = number of ways up to the current column. */
    seq_t dp[128] = {0};
    seq_t newdp[128];
    dp[0] = (seq_t)1;          /* start with no incoming dominoes */

    for (idx_t col = 0; col < n; ++col) {
        /* zero newdp */
        for (int i = 0; i < 128; ++i) newdp[i] = (seq_t)0;

        /* choose missing flag: only the first column when n is odd */
        int missing = ((n & 1U) && (col == 0)) ? 1 : 0;
        int (*T)[128] = trans[missing];

        for (int cur = 0; cur < 128; ++cur) {
            seq_t val = dp[cur];
            if (val == (seq_t)0) continue;
            for (int nxt = 0; nxt < 128; ++nxt) {
                int cnt = T[cur][nxt];
                if (cnt == 0) continue;
                for (int k = 0; k < cnt; ++k)
                    newdp[nxt] += val;
            }
        }

        /* copy newdp to dp */
        for (int i = 0; i < 128; ++i)
            dp[i] = newdp[i];
    }
    return dp[0];
}
