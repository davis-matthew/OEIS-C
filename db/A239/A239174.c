/*
 * A239174  Number of (n+1)X(4+1) 0..2 arrays with no element greater than all
 *          horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A239174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A239174 via dynamic programming over valid row patterns. */
#include <stdbool.h>
#include <stdint.h>

/* Number of (n+1)×5 arrays with entries 0..2 satisfying the constraints
   that no element is greater than all horizontal neighbours and no element
   is equal to all vertical neighbours. */

enum { WIDTH = 5, ALPH = 3, TOT = ALPH * ALPH * ALPH * ALPH * ALPH }; /* 243 */

seq_t a(idx_t n)
{
    /* Enumerate all horizontal patterns that satisfy the horizontal
       constraint.  The number of such patterns is a small constant (≈150). */
    static bool init = false;
    static uint8_t pat[TOT][WIDTH];
    static int R = 0;               /* number of valid patterns */

    if (!init) {
        int cnt = 0;
        for (int v = 0; v < TOT; ++v) {
            int t = v;
            for (int pos = WIDTH - 1; pos >= 0; --pos) {
                pat[cnt][pos] = (uint8_t)(t % ALPH);
                t /= ALPH;
            }
            bool good = true;
            /* endpoint rules */
            if (pat[cnt][0] > pat[cnt][1]) good = false;
            if (pat[cnt][WIDTH - 1] > pat[cnt][WIDTH - 2]) good = false;
            /* interior peaks */
            for (int j = 1; j <= WIDTH - 2 && good; ++j)
                if (pat[cnt][j] > pat[cnt][j - 1] &&
                    pat[cnt][j] > pat[cnt][j + 1])
                    good = false;
            if (good) {
                ++cnt;
            }
        }
        R = cnt;
        init = true;
    }

    /* Compatibility of adjacent rows: no equal entries in the same column. */
    static char compat[TOT][TOT];
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < R; ++j) {
            char ok = 1;
            for (int k = 0; k < WIDTH; ++k) {
                if (pat[i][k] == pat[j][k]) { ok = 0; break; }
            }
            compat[i][j] = ok;
        }
    }

    /* Triplet compatibility: for each column not all three equal. */
    static int next_tbl[TOT][TOT][TOT];
    static uint16_t next_cnt[TOT][TOT];
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < R; ++j) {
            int c = 0;
            for (int l = 0; l < R; ++l) {
                bool ok = true;
                for (int k = 0; k < WIDTH; ++k) {
                    if (pat[i][k] == pat[j][k] && pat[j][k] == pat[l][k]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    next_tbl[i][j][c++] = l;
                }
            }
            next_cnt[i][j] = (uint16_t)c;
        }
    }

    /* Dynamic programming over rows. */
    static seq_t dp[TOT][TOT];
    static seq_t dp2[TOT][TOT];

    /* clear dp */
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < R; ++j)
            dp[i][j] = 0;

    /* initialise with the first two rows (top boundary). */
    seq_t ONE = 1;
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < R; ++j)
            if (compat[i][j])
                dp[i][j] = ONE;

    if (n == 1) { /* only two rows */
        seq_t ans = 0;
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < R; ++j)
                if (compat[i][j])
                    ans += dp[i][j];
        return ans;
    }

    /* iterate over remaining rows (row index 2 … n). */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < R; ++j)
                dp2[i][j] = 0;

        for (int i = 0; i < R; ++i) {
            for (int j = 0; j < R; ++j) {
                seq_t val = dp[i][j];
                if (val == 0) continue;
                int c = next_cnt[i][j];
                for (int t = 0; t < c; ++t) {
                    int l = next_tbl[i][j][t];
                    dp2[j][l] += val;
                }
            }
        }

        for (int i = 0; i < R; ++i)
            for (int j = 0; j < R; ++j)
                dp[i][j] = dp2[i][j];
    }

    /* bottom boundary: last two rows must differ. */
    seq_t result = 0;
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < R; ++j)
            if (compat[i][j])
                result += dp[i][j];

    return result;
}
