/*
 * A207712  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays avoiding 000 and 001
 *            horizontally and 001 and 100 vertically. Computed by a dynamic
 *            programming state machine on the last two rows
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a is called with n ≥ 1 (offset is 1).  It returns the
   exact count using the global integer type seq_t.  The computation
   works for any seq_t (8/16/32/64/128‑bit or arbitrary‑precision) and
   takes O(n) time and negligible memory. */

#include <stddef.h>
#include <stdbool.h>

static const int ALLOWED_ROWS[6] = {2, 3, 4, 5, 6, 7};     /* 010, 011, 100, 101, 110, 111 */

/* Pre‑computed table: valid[p2][p1][c] == true if rows p2 (row i-2),
   p1 (row i-1) and c (row i) satisfy the vertical restrictions. */
static bool VALID_TABLE[6][6][6];
static bool VALID_INI = false;

static void init_valid_table(void)
{
    if (VALID_INI) return;
    for (int a = 0; a < 6; ++a) {
        int r2 = ALLOWED_ROWS[a];
        for (int b = 0; b < 6; ++b) {
            int r1 = ALLOWED_ROWS[b];
            for (int c = 0; c < 6; ++c) {
                int rc = ALLOWED_ROWS[c];
                bool ok = true;
                for (int col = 0; col < 3; ++col) {
                    int b2 = (r2 >> col) & 1;
                    int b1 = (r1 >> col) & 1;
                    int bc = (rc >> col) & 1;
                    if ((b2 == 0 && b1 == 0 && bc == 1)   /* 0 0 1 vertically forbidden */
                        || (b2 == 1 && b1 == 0 && bc == 0)) { /* 1 0 0 forbidden */
                        ok = false;
                        break;
                    }
                }
                VALID_TABLE[a][b][c] = ok;
            }
        }
    }
    VALID_INI = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined but harmless */
    if (n == 1) return 6;              /* only the 6 allowed rows */
    if (n == 2) return 36;             /* 6 × 6 pairs of rows, vertical rules not yet applicable */

    init_valid_table();                /* ensure VALID_TABLE ready */

    /* DP arrays: dp[prev2][prev1] = number of ways for rows up to current
       with last two rows being the indices of ALLOWED_ROWS. */
    seq_t dp[6][6] = {0};
    seq_t ndp[6][6] = {0};

    /* Initialise for i = 2 (first two rows): all 36 pairs are legal. */
    for (int i2 = 0; i2 < 6; ++i2)
        for (int i1 = 0; i1 < 6; ++i1)
            dp[i2][i1] = 1;

    /* Extend rows from i = 3 to n */
    for (idx_t i = 3; i <= n; ++i) {
        /* zero ndp */
        for (int a = 0; a < 6; ++a)
            for (int b = 0; b < 6; ++b)
                ndp[a][b] = 0;

        for (int a = 0; a < 6; ++a) {
            for (int b = 0; b < 6; ++b) {
                seq_t ways = dp[a][b];
                if (!ways) continue;
                for (int c = 0; c < 6; ++c) {
                    if (!VALID_TABLE[a][b][c]) continue;
                    ndp[b][c] += ways;
                }
            }
        }
        /* copy ndp back to dp */
        for (int a = 0; a < 6; ++a)
            for (int b = 0; b < 6; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* Sum all states for total number of arrays of length n */
    seq_t total = 0;
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            total += dp[a][b];

    return total;
}
