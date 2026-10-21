/*
 * A231747  Number of n X 2 0..2 arrays with no element less than a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors.
 * https://oeis.org/A231747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over rows to compute A231747 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* 9 possible row states: (col0,col1) with values 0..2. */
static const int state_vals[9][2] = {
    {0,0},
    {0,1},
    {0,2},
    {1,0},
    {1,1},
    {1,2},
    {2,0},
    {2,1},
    {2,2}
};

/* tables for validity checks */
static bool init_done = false;
static bool valid_top[9][9];
static bool valid_middle[9][9][9];
static bool valid_bottom[9][9];

/* check whether a row satisfies the rule given neighbors */
static inline bool row_valid_index(int cur,
                                   int up, bool has_up,
                                   int down, bool has_down)
{
    int a = state_vals[cur][0];
    int b = state_vals[cur][1];

    /* left cell */
    int g_left = 0;
    int neigh_left = 1;               /* horizontal right cell */
    if (b > a) g_left++;
    if (has_up) {
        if (state_vals[up][0] > a) g_left++;
        if (state_vals[up][1] > a) g_left++;
        neigh_left += 2;
    }
    if (has_down) {
        if (state_vals[down][0] > a) g_left++;
        if (state_vals[down][1] > a) g_left++;
        neigh_left += 2;
    }
    if (g_left > neigh_left/2) return false;

    /* right cell */
    int a_r = b;
    int g_right = 0;
    int neigh_right = 1;               /* horizontal left cell */
    if (state_vals[cur][0] > a_r) g_right++;
    if (has_up) {
        if (state_vals[up][0] > a_r) g_right++;
        if (state_vals[up][1] > a_r) g_right++;
        neigh_right += 2;
    }
    if (has_down) {
        if (state_vals[down][0] > a_r) g_right++;
        if (state_vals[down][1] > a_r) g_right++;
        neigh_right += 2;
    }
    if (g_right > neigh_right/2) return false;

    return true;
}

/* initialise validity tables once */
static void init_tables(void)
{
    if (init_done) return;
    for (int cur = 0; cur < 9; ++cur) {
        for (int next = 0; next < 9; ++next) {
            valid_top[cur][next] = row_valid_index(cur, -1, false, next, true);
        }
    }
    for (int prev = 0; prev < 9; ++prev)
        for (int cur = 0; cur < 9; ++cur) {
            valid_bottom[prev][cur] = row_valid_index(cur, prev, true, -1, false);
        }
    for (int prev = 0; prev < 9; ++prev)
        for (int cur = 0; cur < 9; ++cur)
            for (int next = 0; next < 9; ++next) {
                valid_middle[prev][cur][next] =
                    row_valid_index(cur, prev, true, next, true);
            }
    init_done = true;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_tables();

    /* special case n == 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int s = 0; s < 9; ++s) {
            if (row_valid_index(s, -1, false, -1, false))
                ans += (seq_t)1;
        }
        return ans;
    }

    /* DP arrays: dp[prev][cur] holds counts for last two rows */
    static seq_t dp[9][9];
    static seq_t next_dp[9][9];
    memset(dp, 0, sizeof(dp));

    /* initialise for first two rows */
    for (int prev = 0; prev < 9; ++prev)
        for (int cur = 0; cur < 9; ++cur)
            if (valid_top[prev][cur])
                dp[prev][cur] += (seq_t)1;

    /* if n==2, directly sum with bottom validity */
    if (n == 2) {
        seq_t ans = 0;
        for (int prev = 0; prev < 9; ++prev)
            for (int cur = 0; cur < 9; ++cur)
                if (valid_bottom[prev][cur])
                    ans += dp[prev][cur];
        return ans;
    }

    /* rows 3 .. n */
    for (idx_t i = 3; i <= n; ++i) {
        memset(next_dp, 0, sizeof(next_dp));
        for (int prev = 0; prev < 9; ++prev) {
            for (int cur = 0; cur < 9; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                for (int next = 0; next < 9; ++next) {
                    if (valid_middle[prev][cur][next]) {
                        next_dp[cur][next] += ways;
                    }
                }
            }
        }
        /* copy next_dp into dp */
        memcpy(dp, next_dp, sizeof(dp));
    }

    /* final sum: last row must satisfy bottom condition */
    seq_t ans = 0;
    for (int prev = 0; prev < 9; ++prev) {
        for (int cur = 0; cur < 9; ++cur) {
            if (valid_bottom[prev][cur])
                ans += dp[prev][cur];
        }
    }
    return ans;
}
