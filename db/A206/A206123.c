/*
 * A206123  Number of (n+1)X4 0..2 arrays with every 2X2 subblock having
 *          nonzero determinant and having the same number of clockwise edge
 *          increases as its horizontal and vertical neighbors.
 * https://oeis.org/A206123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function implements the DP described in the OEIS entry:
 *            a(n) = number of (n+1)×4 arrays over {0,1,2} such that every 2×2
 *            subblock has non-zero determinant and all 2×2 subblocks have the
 *            same number of clockwise edge increases. The algorithm
 *            pre‑computes compatibility between all 3^4 rows and...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define N_ROWS 81      /* 3^4 possible rows */
#define N_C     5      /* possible values of C: 0..4 */

static signed char compat[N_ROWS][N_ROWS] = {0};
static unsigned char indeg[N_ROWS][N_C] = {0};
static unsigned char outg[N_C][N_ROWS][N_ROWS] = {0};
static unsigned char outg_cnt[N_C][N_ROWS] = {0};

/* Pre‑compute compatibility data (one‑time, thread‑safe) */
static void prepare(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    /* generate all rows as 4‑digit base‑3 numbers */
    unsigned char rows[N_ROWS][4];
    for (int idx = 0; idx < N_ROWS; ++idx) {
        int v = idx;
        for (int k = 0; k < 4; ++k) {
            rows[idx][k] = (unsigned char)(v % 3);
            v /= 3;
        }
    }

    /* compatibility between every ordered pair of rows */
    for (int i = 0; i < N_ROWS; ++i) {
        for (int j = 0; j < N_ROWS; ++j) {
            int C = -1;
            int ok = 1;
            for (int col = 0; col < 3; ++col) {
                unsigned char a = rows[i][col];
                unsigned char b = rows[i][col + 1];
                unsigned char c = rows[j][col];
                unsigned char d = rows[j][col + 1];

                int det = (int)a * (int)d - (int)b * (int)c;
                if (det == 0) { ok = 0; break; }

                int inc = 0;
                inc += (b > a);
                inc += (d > b);
                inc += (c > d);
                inc += (a > c);

                if (C == -1) C = inc;
                else if (C != inc) { ok = 0; break; }
            }
            if (ok) {
                compat[i][j] = (signed char)C;
                indeg[j][C] += 1;
                outg_cnt[C][i]++;
                /* store in adjacency later */
            } else {
                compat[i][j] = -1;
            }
        }
    }

    /* Build adjacency lists from compat matrix */
    /* We'll first count how many destinations each (C,src) has
       then fill the outg array using a second pass. */
    int pos[N_C][N_ROWS] = {0}; /* current insertion positions */

    for (int i = 0; i < N_ROWS; ++i) {
        for (int j = 0; j < N_ROWS; ++j) {
            int C = compat[i][j];
            if (C != -1) {
                outg[C][i][pos[C][i]++] = (unsigned char)j;
            }
        }
    }
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    prepare();

    /* For n = 1 we need a single transition (two rows) */
    if (n == 1) {
        seq_t total = 0;
        for (int C = 0; C < N_C; ++C) {
            for (int j = 0; j < N_ROWS; ++j) {
                total += (seq_t)indeg[j][C];
            }
        }
        return total;
    }

    /* DP arrays: dp_prev[ C ][ row ] and dp_next[ C ][ row ] */
    seq_t dp_prev[N_C][N_ROWS] = {0};
    seq_t dp_next[N_C][N_ROWS] = {0};

    /* Initialize for length 2 rows (one transition) */
    for (int C = 0; C < N_C; ++C) {
        for (int i = 0; i < N_ROWS; ++i) {
            dp_prev[C][i] = (seq_t)indeg[i][C];
        }
    }

    /* Add rows one by one */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero dp_next */
        for (int C = 0; C < N_C; ++C) {
            for (int i = 0; i < N_ROWS; ++i) {
                dp_next[C][i] = 0;
            }
        }

        /* Transition: for each src row, for each C, add to all dsts */
        for (int src = 0; src < N_ROWS; ++src) {
            for (int C = 0; C < N_C; ++C) {
                seq_t val = dp_prev[C][src];
                if (val == 0) continue;
                int cnt = outg_cnt[C][src];
                for (int k = 0; k < cnt; ++k) {
                    int dst = outg[C][src][k];
                    dp_next[C][dst] += val;
                }
            }
        }

        /* Prepare for next iteration */
        for (int C = 0; C < N_C; ++C) {
            for (int i = 0; i < N_ROWS; ++i) {
                dp_prev[C][i] = dp_next[C][i];
            }
        }
    }

    /* Sum all counts for the final row */
    seq_t result = 0;
    for (int C = 0; C < N_C; ++C) {
        for (int i = 0; i < N_ROWS; ++i) {
            result += dp_prev[C][i];
        }
    }
    return result;
}
