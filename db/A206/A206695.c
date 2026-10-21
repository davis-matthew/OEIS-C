/*
 * A206695  Number of nX3 0..2 arrays avoiding the pattern z-2 z-1 z in any
 *          row or column.
 * https://oeis.org/A206695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2} avoiding the successive
 *            pattern 0,1,2 in any row or column. Computed by dynamic
 *            programming over the last two rows. a(1) = 26, a(2) = 676, a(3)
 *            = 15712, …
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define NROW 27            /* 3^3 possible rows  */
#define STATE (NROW * NROW)/* pair of last two rows     */
#define PATTERN_ROW 5      /* index of the row [0,1,2] */

/* convert an index 0..26 to its ternary digits (row of 3 cells) */
static void row_of_index(int idx, int out[3]) {
    int t = idx;
    out[2] = t % 3;  t /= 3;
    out[1] = t % 3;  t /= 3;
    out[0] = t % 3;
}

/* static tables initialized only once */

/* rows[i] gives the three digits of row i */
static int rows[NROW][3];
/* adjacency: for each state (last two rows) list possible next rows
   and the number of successors */
static unsigned char adj[STATE][NROW];
static unsigned char adj_deg[STATE];
/* memoisation buffer */
static seq_t buffer[2][STATE];
static int initialized = 0;

seq_t a(idx_t n)
{
    /* initialise tables on first call */
    if (!initialized) {
        int i;
        /* rows */
        for (i = 0; i < NROW; ++i) row_of_index(i, rows[i]);

        /* adjacency lists */
        for (i = 0; i < STATE; ++i) {
            int L2 = i / NROW;
            int L1 = i % NROW;
            if (L2 == PATTERN_ROW || L1 == PATTERN_ROW) {
                adj_deg[i] = 0;
                continue;
            }
            unsigned char deg = 0;
            int r;
            for (r = 0; r < NROW; ++r) {
                if (r == PATTERN_ROW) continue;
                /* check vertical pattern 0,1,2 in all three columns */
                int ok = 1;
                int c;
                for (c = 0; c < 3; ++c) {
                    if (rows[L2][c] == 0 &&
                        rows[L1][c] == 1 &&
                        rows[r][c]   == 2) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    adj[i][deg++] = (unsigned char)r;
                }
            }
            adj_deg[i] = deg;
        }
        initialized = 1;
    }

    /* trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)26;          /* 27-1 */
    if (n == 2) return (seq_t)676;         /* 26*26 */

    int cur = 0, nxt = 1;
    int i;
    /* reset current buffer */
    for (i = 0; i < STATE; ++i) buffer[cur][i] = (seq_t)0;

    /* initialise DP for two rows: all admissible pairs of rows */
    for (int L2 = 0; L2 < NROW; ++L2) {
        if (L2 == PATTERN_ROW) continue;
        for (int L1 = 0; L1 < NROW; ++L1) {
            if (L1 == PATTERN_ROW) continue;
            int state = L2 * NROW + L1;
            buffer[cur][state] = (seq_t)1;
        }
    }

    /* iterate rows 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero next buffer */
        for (i = 0; i < STATE; ++i) buffer[nxt][i] = (seq_t)0;

        for (i = 0; i < STATE; ++i) {
            seq_t ways = buffer[cur][i];
            if (!ways) continue;
            int L1 = i % NROW;
            unsigned char deg = adj_deg[i];
            unsigned char k;
            for (k = 0; k < deg; ++k) {
                int R = adj[i][k];
                int new_state = L1 * NROW + R;
                buffer[nxt][new_state] += ways;
            }
        }
        /* swap buffers */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all ways for final row configuration */
    seq_t total = (seq_t)0;
    for (i = 0; i < STATE; ++i) total += buffer[cur][i];

    return total;
}
