/*
 * A183376  Number of nX3 binary arrays with no element equal to the mod 3 sum
 *          of its king-move neighbors.
 * https://oeis.org/A183376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of A183376:
 *  Number of n×3 binary arrays with no element equal to the mod‑3 sum of
 *  its king‑move neighbors.
 *
 *  The function uses a simple DP over rows.  For each column we store a
 *  bit (0 or 1).  A row state is a 3‑bit integer (0..7).  When we add a
 *  new row we ensure that all cells in the current middle row are valid
 *  with respect to the previous and next rows.  This is equivalent to
 *  checking that every element differs from the mod‑3 sum of its
 *  neighbors.
 *
 *  For 'n'=1 the algorithm counts all 3‑bit rows that satisfy the
 *  condition with zero dummy rows above and below.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle an empty array (not used in OEIS, but harmless). */
    if (n == 0) return 0;

    /* Pre‑compute the validity table for triples of rows. */
    static bool valid[8][8][8];
    static bool init = false;
    if (!init) {
        for (int up = 0; up < 8; ++up) {
            for (int cur = 0; cur < 8; ++cur) {
                for (int down = 0; down < 8; ++down) {
                    bool ok = true;
                    for (int j = 0; j < 3; ++j) {          /* column j */
                        int cur_bit = (cur >> j) & 1;
                        int sum = 0;
                        for (int dy = -1; dy <= 1; ++dy) {
                            for (int dx = -1; dx <= 1; ++dx) {
                                if (dy == 0 && dx == 0) continue;   /* self */
                                int col = j + dx;
                                if (col < 0 || col > 2) continue;   /* outside */
                                int row_bits;
                                if (dy == -1)      row_bits = up;
                                else if (dy == 0)  row_bits = cur;
                                else               row_bits = down;
                                sum += (row_bits >> col) & 1;
                            }
                        }
                        if (cur_bit == (sum % 3)) { ok = false; break; }
                    }
                    valid[up][cur][down] = ok;
                }
            }
        }
        init = true;
    }

    /* DP tables: dp[prev][cur] holds the count of partial arrays
     * ending with rows (prev, cur). */
    seq_t dp[8][8] = {0};
    seq_t ndp[8][8] = {0};

    /* Fill the first row: previous row is the dummy row of zeros. */
    for (int r = 0; r < 8; ++r)
        dp[0][r] = 1;

    /* Add rows 1 .. n-1. */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero new DP table. */
        for (int up = 0; up < 8; ++up)
            for (int cur = 0; cur < 8; ++cur)
                ndp[up][cur] = 0;

        /* Transition. */
        for (int up = 0; up < 8; ++up) {
            for (int cur = 0; cur < 8; ++cur) {
                seq_t cnt = dp[up][cur];
                if (cnt == 0) continue;
                for (int down = 0; down < 8; ++down) {
                    if (!valid[up][cur][down]) continue;
                    ndp[cur][down] += cnt;
                }
            }
        }

        /* Copy ndp back to dp for next iteration. */
        for (int up = 0; up < 8; ++up)
            for (int cur = 0; cur < 8; ++cur)
                dp[up][cur] = ndp[up][cur];
    }

    /* Accept arrays whose last row is valid against a dummy zero row below. */
    seq_t result = 0;
    for (int up = 0; up < 8; ++up) {
        for (int cur = 0; cur < 8; ++cur) {
            if (!valid[up][cur][0]) continue;
            result += dp[up][cur];
        }
    }

    return result;
}
