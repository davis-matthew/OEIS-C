/*
 * A200573  Number of n X 2 0..2 arrays with no average of any element and its
 *          horizontal and vertical neighbors equal to one.
 * https://oeis.org/A200573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200573: Count of n × 2 arrays over {0,1,2}
   with no cell having the average of itself and its horizontal and
   vertical neighbors equal to 1.  Computed by dynamic programming
   over rows. */

#define NONE 9       /* sentinel index for “no row” */

static const unsigned char pat[9][2] = {   /* all 3² patterns */
    {0,0}, {0,1}, {0,2},
    {1,0}, {1,1}, {1,2},
    {2,0}, {2,1}, {2,2}
};

/* Check that all cells in row `mid` are valid when
   its upper neighbor is `up` and lower neighbor is `down`.
   `up` or `down` may be NONE meaning that neighbor does not exist. */
static int row_ok(int up, int mid, int down)
{
    int up_present = up != NONE;
    int down_present = down != NONE;

    /* check both columns */
    for (int j = 0; j < 2; ++j) {
        int left = pat[mid][1 - j];
        int sum = pat[mid][j] + left;
        if (up_present)   sum += pat[up][j];
        if (down_present) sum += pat[down][j];
        int cnt = 2 + up_present + down_present;  /* self, horizontal, up, down */
        if (sum == cnt) return 0;                  /* forbidden average */
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n<1 */

    /* Special case n==1: only one row, no up or down neighbors. */
    if (n == 1) {
        seq_t res = 0;
        for (int p = 0; p < 9; ++p)
            if (row_ok(NONE, p, NONE)) ++res;
        return res;
    }

    /* DP arrays: dp[up][cur] counts ways up to current row,
       where `up` is the pattern of the previous row (or NONE for first row),
       and `cur` is the pattern of the current row. */
    static seq_t dp[10][10];
    static seq_t next[10][10];

    /* initialise dp for the first row: up == NONE */
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            dp[i][j] = 0;
    for (int p = 0; p < 9; ++p)
        dp[NONE][p] = 1;

    /* process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                next[i][j] = 0;

        for (int up = 0; up <= 9; ++up) {
            for (int cur = 0; cur < 9; ++cur) {
                seq_t ways = dp[up][cur];
                if (!ways) continue;
                for (int nxt = 0; nxt < 9; ++nxt) {
                    if (!row_ok(up, cur, nxt)) continue;
                    next[cur][nxt] += ways;
                }
            }
        }
        /* swap dp and next */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                dp[i][j] = next[i][j];
    }

    /* after processing all rows, dp[up][cur] contains valid arrays
       where `up` = row n‑1, `cur` = row n, and all rows except the last
       have been validated.  Validate the final row (no down neighbor). */
    seq_t total = 0;
    for (int up = 0; up < 9; ++up)
        for (int cur = 0; cur < 9; ++cur)
            if (dp[up][cur] && row_ok(up, cur, NONE))
                total += dp[up][cur];

    return total;
}
