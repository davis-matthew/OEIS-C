/*
 * A207903  Number of nX3 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A207903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 31, u64 n <= 71, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207903: Number of n×3 0/1 arrays avoiding
 *  horizontally 000 and 010, vertically 000 and 011.
 *
 *  Computed by dynamic programming over the last two rows.
 *  O(n) time with a constant 6×6 state table.
 *  Initial values: a(1)=6, a(2)=36.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allowed single rows (exclude 000 and 010). */
    const int allowed[6] = {1, 3, 4, 5, 6, 7};

    /* Pre‑compute which triples of rows satisfy the vertical constraints. */
    static bool valid[6][6][6];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                for (int k = 0; k < 6; ++k) {
                    int a_row = allowed[i];
                    int b_row = allowed[j];
                    int c_row = allowed[k];
                    bool ok = true;
                    for (int col = 0; col < 3 && ok; ++col) {
                        bool bitA = (a_row >> col) & 1;
                        bool bitB = (b_row >> col) & 1;
                        bool bitC = (c_row >> col) & 1;
                        if (!bitA && !bitB && !bitC)          /* 000 */
                            ok = false;
                        else if (!bitA && bitB && bitC)       /* 011 */
                            ok = false;
                    }
                    valid[i][j][k] = ok;
                }
            }
        }
        init = true;
    }

    if (n == 0) return (seq_t)0;          /* out of offset – harmless */

    if (n == 1) return (seq_t)6;          /* a(1) */
    if (n == 2) return (seq_t)36;         /* a(2) */

    /* DP arrays: dp[i][j] – number of ways where the last two rows are
     * allowed[i] (row n-1) and allowed[j] (row n).   */
    seq_t dp[6][6];
    seq_t dp_new[6][6];

    /* Initialize for the first two rows: all 6×6 pairs are admissible. */
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            dp[i][j] = 1;

    /* Process rows 3 … n. */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dp_new[i][j] = 0;

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                seq_t cur = dp[i][j];
                if (cur == 0) continue;
                for (int k = 0; k < 6; ++k) {
                    if (!valid[i][j][k]) continue;
                    dp_new[j][k] += cur;
                }
            }
        }

        /* Copy back for next iteration. */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dp[i][j] = dp_new[i][j];
    }

    /* Sum over all final states. */
    seq_t ans = 0;
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            ans += dp[i][j];

    return ans;
}
