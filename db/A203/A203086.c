/*
 * A203086  Number of (n+2)X5 binary arrays avoiding patterns 000 and 101 in
 *          rows and columns.
 * https://oeis.org/A203086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 31, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: uses a transfer‑matrix dynamic programming to count
 * (n+2)×5 binary arrays that avoid the patterns 000 and 101 in rows
 * and columns.  The DP keeps track of the last two rows.
 * Complexity: O((n+2)·M³) where M is the number of 5‑bit rows that
 * themselves avoid 000 and 101 (M ≈ 15).  This is well below the
 * 2‑second limit for n≤10000. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    const idx_t totalRows = n + 2;  /* total number of rows (n≥1 ⇒ ≥3) */

    /* Pre‑computation that depends only on the pattern size */
    static bool initialized = false;
    static int allowed_rows[32];
    static int allowed_count;
    static bool compat[32][32][32];

    if (!initialized) {
        /* All 5‑bit rows that avoid 000 and 101 horizontally */
        allowed_count = 0;
        for (int r = 0; r < 32; ++r) {
            bool ok = true;
            for (int j = 0; j <= 2; ++j) {
                int bits = (r >> j) & 7;   /* three consecutive bits */
                if (bits == 0 || bits == 5) { ok = false; break; }
            }
            if (ok) allowed_rows[allowed_count++] = r;
        }

        /* Compute compatibility of three consecutive rows (vertical check) */
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                for (int r = 0; r < 32; ++r) {
                    bool good = true;
                    for (int j = 0; j < 5; ++j) {
                        int pat = ((a >> j) & 1) << 2 |
                                  ((b >> j) & 1) << 1 |
                                  ((r >> j) & 1);
                        if (pat == 0 || pat == 5) { good = false; break; }
                    }
                    compat[a][b][r] = good;
                }
            }
        }
        initialized = true;
    }

    /* DP tables: dp_prev[a][b] = number of ways with last two rows a,b */
    seq_t dp_prev[32][32];
    seq_t dp_next[32][32];

    /* Clear dp_prev */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp_prev[i][j] = 0;

    /* Initialize with all pairs of allowed rows (first two rows) */
    for (int ia = 0; ia < allowed_count; ++ia) {
        int arow = allowed_rows[ia];
        for (int ib = 0; ib < allowed_count; ++ib) {
            int brow = allowed_rows[ib];
            dp_prev[arow][brow] = 1;
        }
    }

    /* Process remaining rows, starting from row 3 */
    for (idx_t row = 3; row <= totalRows; ++row) {
        /* Clear dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int ia = 0; ia < allowed_count; ++ia) {
            int arow = allowed_rows[ia];
            for (int ib = 0; ib < allowed_count; ++ib) {
                int brow = allowed_rows[ib];
                seq_t val = dp_prev[arow][brow];
                if (val == 0) continue;

                for (int ir = 0; ir < allowed_count; ++ir) {
                    int crow = allowed_rows[ir];
                    if (!compat[arow][brow][crow]) continue;
                    dp_next[brow][crow] += val;
                }
            }
        }

        /* Move dp_next to dp_prev for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_prev[i][j] = dp_next[i][j];
    }

    /* Sum over all final states */
    seq_t result = 0;
    for (int ia = 0; ia < 32; ++ia)
        for (int ib = 0; ib < 32; ++ib)
            result += dp_prev[ia][ib];

    return result;
}
