/*
 * A233945  Number of (n+1)X(4+1) 0..2 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 4.
 * https://oeis.org/A233945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays with entries 0,1,2 such that
 *            every 2×2 subblock has the sum of the absolute values of its
 *            edge differences equal to 4
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

enum { ROWS = 243 };               /* 3^5 possible rows */

static uint8_t row_val[ROWS][5];    /* base‑3 representation of a row */
static uint8_t deg[ROWS];           /* number of admissible next rows */
static uint16_t nbr[ROWS][ROWS];    /* adjacency list stored densely */

/* Build the row representations and the adjacency list once. */
static void build_graph(void)
{
    /* Pre‑compute row digits in base 3. */
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int c = 0; c < 5; ++c) {
            row_val[r][c] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* For each pair of rows, test all four 2×2 blocks. */
    for (int i = 0; i < ROWS; ++i) {
        int d = 0;
        for (int j = 0; j < ROWS; ++j) {
            int ok = 1;
            for (int col = 0; col < 4; ++col) {
                int a = row_val[i][col];
                int b = row_val[i][col + 1];
                int c = row_val[j][col];
                int d2 = row_val[j][col + 1];
                int sum =
                    abs(a - b) + abs(a - c) + abs(d2 - b) + abs(d2 - c);
                if (sum != 4) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                nbr[i][d] = (uint16_t)j;
                ++d;
            }
        }
        deg[i] = (uint8_t)d;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Lazy initialisation of the adjacency graph. */
    static int graph_initialised = 0;
    if (!graph_initialised) {
        build_graph();
        graph_initialised = 1;
    }

    static seq_t dp_prev[ROWS];
    static seq_t dp_cur[ROWS];

    /* Initialise counts for the first row: one way for each row. */
    for (int i = 0; i < ROWS; ++i)
        dp_prev[i] = (seq_t)1;

    /* Perform n transitions (creating n+1 rows). */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero current dp array */
        for (int i = 0; i < ROWS; ++i)
            dp_cur[i] = (seq_t)0;

        /* transition */
        for (int i = 0; i < ROWS; ++i) {
            seq_t v = dp_prev[i];
            if (v == (seq_t)0) continue;
            for (int k = 0; k < deg[i]; ++k) {
                int j = nbr[i][k];
                dp_cur[j] += v;
            }
        }

        /* prepare for next step: copy dp_cur into dp_prev */
        for (int i = 0; i < ROWS; ++i)
            dp_prev[i] = dp_cur[i];
    }

    /* Sum over all ending rows. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < ROWS; ++i)
        total += dp_prev[i];

    return total;
}
