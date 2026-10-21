/*
 * A207782  Number of n X 5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A207782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------------------------------------------------------
 * OEIS sequence A207782
 * Number of n × 5 0/1 arrays avoiding
 *     horizontal 000, 011
 * and  vertical   000, 101
 * ---------------------------------------------------------------
 * Computation:
 *   Dynamic programming over rows.
 *   State: last two rows (patterns 0..31)
 *   Transition: a new row may be appended if it satisfies the
 *   horizontal constraints and the vertical constraints with
 *   the previous two rows.
 *   Complexity:  O(n · 32²)   (≈ 1 k operations per row)
 * --------------------------------------------------------------- */

#include <stdbool.h>
#define COLS 5
#define PAT  (1 << COLS)        /* 32 possible row patterns */

seq_t a(idx_t n)
{
    /* ---------- precomputation of legal patterns and transitions ---------- */
    static bool preinit = false;
    static unsigned char H[PAT];                /* horizontal legality */
    static unsigned char allowed_cnt[PAT][PAT];
    static int allowed_list[PAT][PAT][PAT];     /* list of successors */

    if (!preinit) {
        preinit = true;

        /* horizontal legality of each row pattern */
        for (int p = 0; p < PAT; ++p) {
            bool ok = true;
            for (int j = 0; j <= COLS - 3 && ok; ++j) {
                int a0 = (p >> j)       & 1;
                int a1 = (p >> (j + 1)) & 1;
                int a2 = (p >> (j + 2)) & 1;
                if (a0 == 0 && a1 == 0 && a2 == 0) ok = false;   /* 000 */
                if (a0 == 0 && a1 == 1 && a2 == 1) ok = false;   /* 011 */
            }
            H[p] = ok;
        }

        /* transitions */
        for (int p1 = 0; p1 < PAT; ++p1) {
            for (int p2 = 0; p2 < PAT; ++p2) {
                int cnt = 0;
                for (int r = 0; r < PAT; ++r) {
                    if (!H[r]) continue;
                    bool ok = true;
                    for (int j = 0; j < COLS; ++j) {
                        int a0 = (p1 >> j) & 1;
                        int a1 = (p2 >> j) & 1;
                        int a2 = (r  >> j) & 1;
                        if (a0 == 0 && a1 == 0 && a2 == 0) { ok = false; break; }  /* 000 */
                        if (a0 == 1 && a1 == 0 && a2 == 1) { ok = false; break; }  /* 101 */
                    }
                    if (ok) {
                        allowed_list[p1][p2][cnt] = r;
                        ++cnt;
                    }
                }
                allowed_cnt[p1][p2] = cnt;
            }
        }
    }

    /* ---------- DP arrays ---------- */
    static seq_t dp[PAT][PAT];
    static seq_t dp_next[PAT][PAT];

    /* ---------- base cases ---------- */
    if (n == 0) return 0;          /* not used, but safe */
    if (n == 1) {
        seq_t sum = 0;
        for (int p = 0; p < PAT; ++p)
            if (H[p]) ++sum;
        return sum;
    }

    /* initialize dp for first two rows */
    for (int p1 = 0; p1 < PAT; ++p1) {
        for (int p2 = 0; p2 < PAT; ++p2) {
            dp[p1][p2] = (H[p1] && H[p2]) ? 1 : 0;
        }
    }

    if (n == 2) {
        seq_t sum = 0;
        for (int p1 = 0; p1 < PAT; ++p1)
            for (int p2 = 0; p2 < PAT; ++p2)
                sum += dp[p1][p2];
        return sum;
    }

    /* ---------- main DP loop ---------- */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero dp_next */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                dp_next[i][j] = 0;

        for (int p1 = 0; p1 < PAT; ++p1) {
            for (int p2 = 0; p2 < PAT; ++p2) {
                seq_t cur = dp[p1][p2];
                if (cur == 0) continue;
                int cnt = allowed_cnt[p1][p2];
                for (int k = 0; k < cnt; ++k) {
                    int r = allowed_list[p1][p2][k];
                    dp_next[p2][r] += cur;
                }
            }
        }

        /* copy back to dp */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* ---------- final sum ---------- */
    seq_t total = 0;
    for (int p1 = 0; p1 < PAT; ++p1)
        for (int p2 = 0; p2 < PAT; ++p2)
            total += dp[p1][p2];

    return total;
}
