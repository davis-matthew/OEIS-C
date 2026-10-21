/*
 * A205831  Number of (n+1) X 4 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to two.
 * https://oeis.org/A205831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays of digits 0..2 whose every 2×2
 *            subblock has exactly two clockwise edge increases. This uses a
 *            transfer‑matrix dynamic program over the 3^4 = 81 possible rows.
 *            No closed form is known
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    enum { COLS = 4, PATTERNS = 81, MAX_NEIGH = PATTERNS };

    static int row_vals[PATTERNS][COLS];
    static int next_rows[PATTERNS][MAX_NEIGH];
    static unsigned char deg[PATTERNS];
    static int init = 0;

    if (!init) {
        /* enumerate all rows */
        for (int i = 0; i < PATTERNS; ++i) {
            int tmp = i;
            for (int c = 0; c < COLS; ++c) {
                row_vals[i][c] = tmp % 3;
                tmp /= 3;
            }
        }
        /* build adjacency matrix: next_rows[p] lists rows q reachable
           from p in a single step.  deg[p] counts the number of such q. */
        for (int p = 0; p < PATTERNS; ++p) {
            deg[p] = 0;
            for (int q = 0; q < PATTERNS; ++q) {
                int ok = 1;
                for (int j = 0; j < COLS - 1; ++j) {
                    int a0 = row_vals[p][j];
                    int a1 = row_vals[p][j + 1];
                    int a2 = row_vals[q][j];
                    int a3 = row_vals[q][j + 1];
                    int inc = 0;
                    if (a1 > a0) ++inc;
                    if (a3 > a1) ++inc;
                    if (a2 > a3) ++inc;
                    if (a0 > a2) ++inc;
                    if (inc != 2) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    next_rows[p][deg[p]++] = q;
                }
            }
        }
        init = 1;
    }

    seq_t dp_cur[PATTERNS];
    seq_t dp_next[PATTERNS];

    /* DP initialization: any first row is allowed. */
    for (int i = 0; i < PATTERNS; ++i)
        dp_cur[i] = (seq_t)1;

    /* Perform n steps – from the first row to the (n+1)-th row. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < PATTERNS; ++i)
            dp_next[i] = (seq_t)0;
        for (int p = 0; p < PATTERNS; ++p) {
            seq_t v = dp_cur[p];
            if (!v) continue;
            unsigned char d = deg[p];
            for (unsigned char k = 0; k < d; ++k) {
                int q = next_rows[p][k];
                dp_next[q] += v;
            }
        }
        for (int i = 0; i < PATTERNS; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Sum over all ending rows. */
    seq_t sum = 0;
    for (int i = 0; i < PATTERNS; ++i)
        sum += dp_cur[i];
    return sum;
}
