/*
 * A233687  Number of (n+1) X (4+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 10,
 *          and no two adjacent values equal.
 * https://oeis.org/A233687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays over 0..3 with no two adjacent
 *            equal and every 2×2 subblock having the sum of the squares of
 *            the edge differences equal to 10. The function builds a directed
 *            graph of all 324 admissible rows and then does dynamic
 *            programming over that graph. This algorithm works fo...
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Maximum number of admissible rows.
   Each entry has 5 cells and no two adjacent equal → 4·3⁴ = 324. */
#define MAX_ROWS 324

seq_t a(idx_t n)
{
    /* Tables are built only once. */
    static bool initialized = false;

    /* All admissible 5‑wide rows. */
    static unsigned char rows[MAX_ROWS][5];
    /* Number of outgoing edges from each row. */
    static unsigned short degree[MAX_ROWS];
    /* Adjacency list stored densely: neigh[i][k] gives k‑th neighbour
       of row i, for 0 ≤ k < degree[i]. */
    static unsigned short neigh[MAX_ROWS][MAX_ROWS];
    static int row_cnt = 0;

    if (!initialized) {
        /* Generate all valid rows (adjacent cells different). */
        int r = 0;
        for (unsigned char a = 0; a < 4; ++a)
            for (unsigned char b = 0; b < 4; ++b)
                for (unsigned char c = 0; c < 4; ++c)
                    for (unsigned char d = 0; d < 4; ++d)
                        for (unsigned char e = 0; e < 4; ++e) {
                            if (a != b && b != c && c != d && d != e) {
                                rows[r][0] = a;
                                rows[r][1] = b;
                                rows[r][2] = c;
                                rows[r][3] = d;
                                rows[r][4] = e;
                                ++r;
                            }
                        }
        row_cnt = r;          /* should be 324 */

        /* Build directed adjacency: i → j if rows i and j can form a
           valid 2×5 block. */
        for (int i = 0; i < row_cnt; ++i) {
            unsigned short deg = 0;
            for (int j = 0; j < row_cnt; ++j) {
                bool ok = true;
                for (int k = 0; k < 4 && ok; ++k) {
                    int dtop     = (int)rows[i][k+1] - (int)rows[i][k];
                    int dbottom  = (int)rows[j][k+1] - (int)rows[j][k];
                    int dleft    = (int)rows[j][k]   - (int)rows[i][k];
                    int dright   = (int)rows[j][k+1] - (int)rows[i][k+1];

                    int a1 = dtop   >= 0 ? dtop   : -dtop;
                    int a2 = dbottom>= 0 ? dbottom: -dbottom;
                    int a3 = dleft  >= 0 ? dleft  : -dleft;
                    int a4 = dright >= 0 ? dright : -dright;

                    /* Each edge must be non‑zero (no equal adjacent) and
                       at most 2 (otherwise the square would exceed 4). */
                    if (a1 == 0 || a2 == 0 || a3 == 0 || a4 == 0 ||
                        a1 > 2 || a2 > 2 || a3 > 2 || a4 > 2) {
                        ok = false;
                        break;
                    }

                    /* Sum of squares equals 10 ⇔ exactly two differences
                       equal 2 (and the other two equal 1). */
                    int cnt2 = (a1 == 2) + (a2 == 2) + (a3 == 2) + (a4 == 2);
                    if (cnt2 != 2) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    neigh[i][deg++] = (unsigned short)j;
                }
            }
            degree[i] = deg;
        }
        initialized = true;
    }

    /* Dynamic programming over the row graph. */
    seq_t cur[MAX_ROWS];
    seq_t next[MAX_ROWS];
    for (int i = 0; i < row_cnt; ++i) cur[i] = (seq_t)1;   /* first row */

    /* We need n transitions to obtain (n+1) rows. */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < row_cnt; ++i) next[i] = (seq_t)0;

        for (int i = 0; i < row_cnt; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0) continue;
            unsigned short deg = degree[i];
            for (unsigned short k = 0; k < deg; ++k) {
                int j = neigh[i][k];
                next[j] += val;
            }
        }
        for (int i = 0; i < row_cnt; ++i) cur[i] = next[i];
    }

    /* Sum over all possible final rows. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < row_cnt; ++i) total += cur[i];

    return total;
}
