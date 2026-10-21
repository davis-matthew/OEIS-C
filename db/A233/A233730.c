/*
 * A233730  Number of (n+1)X(5+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 6
 *          and no adjacent elements equal.
 * https://oeis.org/A233730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A233730: Number of (n+1)×6 arrays 0..3 such that
   • adjacent elements (horizontally or vertically) are distinct,
   • every 2×2 subblock has the sum of absolute edge differences equal to 6.
   The function uses a dynamic‑programming approach on the graph of valid rows. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* number of possible rows (length 6) with no adjacent equal digits in {0,1,2,3}:
   4 choices for the first, then 3 choices for each of the next 5 positions */
#define ROWS (4 * 243)   /* 972 */

static uint8_t row_digits[ROWS][6];  /* digits of each row */
static int     *adj[ROWS];           /* adjacency list: from row i to row j */
static size_t  adj_deg[ROWS];        /* number of outgoing edges for each row */
static bool    built = false;        /* flag to build the graph only once */

/* Build the row graph once.  This routine is thread‑safe for single‑threaded
   usage as required by the test environment. */
static void build_graph(void)
{
    if (built) return;
    built = true;

    /* Enumerate all valid rows: 0..3 with no two adjacent equal. */
    size_t r = 0;
    for (int a = 0; a < 4; ++a) {
        for (int b = 0; b < 4; ++b) if (b != a) {
            for (int c = 0; c < 4; ++c) if (c != b) {
                for (int d = 0; d < 4; ++d) if (d != c) {
                    for (int e = 0; e < 4; ++e) if (e != d) {
                        for (int f = 0; f < 4; ++f) if (f != e) {
                            uint8_t *dgt = row_digits[r];
                            dgt[0] = a; dgt[1] = b; dgt[2] = c;
                            dgt[3] = d; dgt[4] = e; dgt[5] = f;
                            ++r;
                        }
                    }
                }
            }
        }
    }

    /* First pass: compute the number of outgoing edges for each row. */
    for (size_t i = 0; i < ROWS; ++i) {
        size_t cnt = 0;
        for (size_t j = 0; j < ROWS; ++j) {
            const uint8_t *top = row_digits[i];
            const uint8_t *bot = row_digits[j];
            bool ok = true;
            for (int col = 0; col < 5; ++col) {
                /* vertical adjacency must differ */
                if (bot[col]     == top[col]     ||
                    bot[col + 1] == top[col + 1]) { ok = false; break; }

                /* 2×2 block condition */
                int a = top[col];
                int b = top[col + 1];
                int c = bot[col];
                int d = bot[col + 1];
                int sum = (a > b ? a - b : b - a)
                        + (c > d ? c - d : d - c)
                        + (a > c ? a - c : c - a)
                        + (b > d ? b - d : d - b);
                if (sum != 6) { ok = false; break; }
            }
            if (ok) ++cnt;
        }
        adj_deg[i] = cnt;
    }

    /* Allocate adjacency arrays. */
    for (size_t i = 0; i < ROWS; ++i) {
        adj[i] = adj_deg[i] ? (int *)malloc(adj_deg[i] * sizeof(int))
                            : NULL;
    }

    /* Second pass: fill adjacency lists. */
    size_t *ptr = (size_t *)calloc(ROWS, sizeof(size_t));
    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < ROWS; ++j) {
            const uint8_t *top = row_digits[i];
            const uint8_t *bot = row_digits[j];
            bool ok = true;
            for (int col = 0; col < 5; ++col) {
                if (bot[col]     == top[col]     ||
                    bot[col + 1] == top[col + 1]) { ok = false; break; }

                int a = top[col];
                int b = top[col + 1];
                int c = bot[col];
                int d = bot[col + 1];
                int sum = (a > b ? a - b : b - a)
                        + (c > d ? c - d : d - c)
                        + (a > c ? a - c : c - a)
                        + (b > d ? b - d : d - b);
                if (sum != 6) { ok = false; break; }
            }
            if (ok) {
                int idx = (int)ptr[i]++;
                adj[i][idx] = (int)j;
            }
        }
    }
    free(ptr);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n >= 1 per OEIS offset.  We interpret n as the number of transitions,
       so (n+1) rows result. */
    build_graph();                     /* ensure graph is ready */

    /* DP vectors for current and next row counts. */
    static seq_t dp_cur[ROWS];
    static seq_t dp_nxt[ROWS];

    /* Initialise for a single row: each row is a valid arrangement. */
    for (size_t i = 0; i < ROWS; ++i)
        dp_cur[i] = 1;

    /* Perform n transitions to obtain arrays of length n+1 rows. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Reset next array counter. */
        for (size_t i = 0; i < ROWS; ++i)
            dp_nxt[i] = 0;

        /* Accumulate contributions from each current row to its neighbours. */
        for (size_t i = 0; i < ROWS; ++i) {
            seq_t val = dp_cur[i];
            if (!val) continue;
            size_t deg = adj_deg[i];
            for (size_t k = 0; k < deg; ++k) {
                int j = adj[i][k];
                dp_nxt[j] = dp_nxt[j] + val;
            }
        }

        /* Swap dp_cur and dp_nxt for the next iteration. */
        for (size_t i = 0; i < ROWS; ++i)
            dp_cur[i] = dp_nxt[i];
    }

    /* Sum all final counts to get the total number of arrays. */
    seq_t total = 0;
    for (size_t i = 0; i < ROWS; ++i)
        total = total + dp_cur[i];

    return total;
}
