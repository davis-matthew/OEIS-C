/*
 * A203085  Number of (n+2)X4 binary arrays avoiding patterns 000 and 101 in
 *          rows and columns.
 * https://oeis.org/A203085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 78,
 *            big any n
 * Helpers:   ok_vertical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A203085
 * Using a transfer‑matrix dynamic programme.
 * Rows are built from top to bottom; state = last two rows.
 * Horizontal restrictions (no 000 or 101) are pre‑checked.
 * Vertical restrictions (no 000 or 101 in any three consecutive rows)
 * are enforced during transition.
 * Complexity: O(n) for n up to 10 000 (≈8 10⁵ very small operations). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ok_vertical.h"

static const int rowbits[9] = {2, 3, 4, 6, 7, 9, 12, 14, 15};

/* pre‑computed transition tables */
static int  trans[9][9][9];
static unsigned char trcnt[9][9];
static int initialized = 0;

/* The main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n = 0 */

    /* Initialize transition tables once */
    if (!initialized) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j) {
                int cnt = 0;
                for (int k = 0; k < 9; ++k)
                    if (ok_vertical(rowbits[i], rowbits[j], rowbits[k]))
                        trans[i][j][cnt++] = k;
                trcnt[i][j] = cnt;
            }
        initialized = 1;
    }

    /* DP tables: dp[i][j] = ways with last two rows = i,j */
    static seq_t dp[9][9];
    static seq_t ndp[9][9];

    /* Initial two rows: any two valid rows */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            dp[i][j] = 1;

    seq_t result = 0;
    /* Rows are numbered from 1 to n+2.
       After processing row r, we have r rows. */
    for (idx_t r = 3; r <= n + 2; ++r) {
        /* reset ndp */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                ndp[i][j] = 0;

        /* transition */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j) {
                seq_t val = dp[i][j];
                if (!val) continue;
                unsigned char cnt = trcnt[i][j];
                for (int t = 0; t < cnt; ++t) {
                    int k = trans[i][j][t];
                    ndp[j][k] += val;
                }
            }

        /* copy back to dp */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                dp[i][j] = ndp[i][j];

        if (r - 2 == n) {        /* we have exactly n+2 rows now */
            seq_t sum = 0;
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    sum += dp[i][j];
            result = sum;
            break;
        }
    }
    return result;
}
