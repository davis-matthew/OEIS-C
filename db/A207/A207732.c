/*
 * A207732  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary matrices avoiding the horizontal
 *            patterns 000 and 010 (in any row) and the vertical patterns 011
 *            and 101 (in any column). The function implements this count. The
 *            implementation uses a 5‑by‑2 state DP where each state consists
 *            of the last two column vectors. Transitions...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute valid column patterns and transitions once. */
    static bool init_done = false;
    static int valid_col[32];
    static int valid_list[32];
    static int valcnt;
    static char trans_cnt[32][32];          /* number of next columns */
    static int  trans_next[32][32][32];     /* next column list */

    if (!init_done) {
        /* Find columns that satisfy vertical constraints. */
        int vnum = 0;
        for (int p = 0; p < 32; ++p) {
            bool ok = true;
            for (int r = 0; r <= 2 && ok; ++r) {         /* triples r,r+1,r+2 */
                int a = (p >> r)     & 1;
                int b = (p >> (r+1)) & 1;
                int c = (p >> (r+2)) & 1;
                if ((a == 0 && b == 1 && c == 1) ||
                    (a == 1 && b == 0 && c == 1))
                    ok = false;
            }
            if (ok) {
                valid_col[p] = 1;
                valid_list[vnum++] = p;
            } else
                valid_col[p] = 0;
        }
        valcnt = vnum;

        /* Pre‑compute horizontal transition lists. */
        for (int p2 = 0; p2 < 32; ++p2)
            for (int p1 = 0; p1 < 32; ++p1)
                trans_cnt[p2][p1] = 0;

        for (int i = 0; i < valcnt; ++i) {
            int prev2 = valid_list[i];
            for (int j = 0; j < valcnt; ++j) {
                int prev1 = valid_list[j];
                for (int k = 0; k < valcnt; ++k) {
                    int cur = valid_list[k];
                    bool ok = true;
                    for (int r = 0; r < 5 && ok; ++r) {
                        int a = (prev2 >> r) & 1;
                        int b = (prev1 >> r) & 1;
                        int c = (cur    >> r) & 1;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 0 && b == 1 && c == 0))
                            ok = false;
                    }
                    if (ok) {
                        int idx = trans_cnt[prev2][prev1];
                        trans_next[prev2][prev1][idx] = cur;
                        trans_cnt[prev2][prev1] = idx + 1;
                    }
                }
            }
        }
        init_done = true;
    }

    /* Quick return for very short widths. */
    if (n == 1) return (seq_t)valcnt;
    if (n == 2) return (seq_t)valcnt * (seq_t)valcnt;

    /* dp[prev1][prev2] : number of ways having last two columns
       (prev1 = column n-1, prev2 = column n-2) */
    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* Initialise: first two columns (any valid pair). */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    for (int i = 0; i < valcnt; ++i) {
        int col1 = valid_list[i];          /* column 1 */
        for (int j = 0; j < valcnt; ++j) {
            int col2 = valid_list[j];      /* column 2 */
            dp[col2][col1] = 1;            /* last = col2, second last = col1 */
        }
    }

    /* Process columns 3 .. n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* Zero dp_next. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        /* Transition. */
        for (int prev1 = 0; prev1 < 32; ++prev1) {
            for (int prev2 = 0; prev2 < 32; ++prev2) {
                seq_t ways = dp[prev1][prev2];
                if (ways == 0) continue;
                int last = trans_cnt[prev2][prev1];
                for (int k = 0; k < last; ++k) {
                    int cur = trans_next[prev2][prev1][k];
                    dp_next[cur][prev1] += ways;
                }
            }
        }

        /* Swap dp and dp_next. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Sum over all final states. */
    seq_t total = 0;
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            total += dp[i][j];
    return total;
}
