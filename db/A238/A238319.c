/*
 * A238319  Number of (n+1) X (4+1) 0..2 arrays with no element greater than
 *          all horizontal neighbors or less than all vertical neighbors.
 * https://oeis.org/A238319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A238319 by dynamic programming over
 * all allowed rows.  An array of size (n+1)×5 is valid if no element
 * is greater than all of its horizontal neighbours or less than all of
 * its vertical neighbours.  The algorithm enumerates the 51 admissible
 * rows (those that satisfy the horizontal condition), builds the
 * relation “row j is not smaller in any column than row i” (used for
 * vertical adjacency), and then performs DP over states (prev_row,
 * curr_row).  The result is the number of sequences of (n+1) rows
 * ending with any allowed final row that also respects the vertical
 * condition with its predecessor. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 243 possible rows (3^5) and 51 admissible ones.  All data are
     * built on the first call. */
    enum { ROWS     = 243,
           COLS     = 5,
           ALLOWED  = 51,
           STATE_CNT = ALLOWED * ALLOWED };

    /* static data that is built once */
    static bool built = false;
    static unsigned char row_digits[ROWS][COLS];          /* digits of each row */
    static int allowed_rows[ALLOWED];                    /* indices of allowed rows */
    static int row_to_allowed[ROWS];                     /* mapping original -> allowed (-1 if not) */
    static unsigned char top_ok[ALLOWED][ALLOWED];       /* row_j >= row_i elementwise? */
    static uint16_t next_state[STATE_CNT][ALLOWED];      /* destination state indices */
    static unsigned char next_cnt[STATE_CNT];            /* number of outgoing edges per state */

    /* DP arrays (two layers) */
    static seq_t dp0[STATE_CNT];
    static seq_t dp1[STATE_CNT];
    static seq_t *dp_cur = dp0;
    static seq_t *dp_next = dp1;

    if (!built)
    {
        /* 1. compute digits of all rows in base 3 */
        for (int r = 0; r < ROWS; ++r) {
            int x = r;
            for (int pos = COLS-1; pos >= 0; --pos) {
                row_digits[r][pos] = (unsigned char)(x % 3);
                x /= 3;
            }
        }

        /* 2. find rows that satisfy the horizontal peak restriction */
        for (int r = 0; r < ROWS; ++r) row_to_allowed[r] = -1;
        int cnt_allowed = 0;
        for (int r = 0; r < ROWS; ++r) {
            bool ok = true;
            for (int j = 0; j < COLS; ++j) {
                unsigned char v = row_digits[r][j];
                bool left  = j > 0;
                bool right = j < COLS-1;
                if (left) {
                    unsigned char l = row_digits[r][j-1];
                    if (right) {
                        unsigned char rgt = row_digits[r][j+1];
                        if (v > l && v > rgt) { ok = false; break; }
                    } else {
                        if (v > l) { ok = false; break; }
                    }
                } else {          /* no left neighbour, only right */
                    if (right) {
                        unsigned char rgt = row_digits[r][j+1];
                        if (v > rgt) { ok = false; break; }
                    }
                }
            }
            if (ok) {
                allowed_rows[cnt_allowed]   = r;
                row_to_allowed[r]           = cnt_allowed;
                ++cnt_allowed;
            }
        }

        /* 3. pre‑compute vertical top condition a >= b elementwise */
        for (int i = 0; i < ALLOWED; ++i) {
            int ri = allowed_rows[i];
            for (int j = 0; j < ALLOWED; ++j) {
                int rj = allowed_rows[j];
                bool ok = true;
                for (int c = 0; c < COLS; ++c) {
                    if (row_digits[ri][c] < row_digits[rj][c]) { ok = false; break; }
                }
                top_ok[i][j] = ok;
            }
        }

        /* 4. pre‑compute adjacency lists for transitions */
        for (int a_row = 0; a_row < ALLOWED; ++a_row) {
            for (int b_row = 0; b_row < ALLOWED; ++b_row) {
                int idx = a_row * ALLOWED + b_row;
                unsigned char out_cnt = 0;
                for (int c_row = 0; c_row < ALLOWED; ++c_row) {
                    bool bad = false;
                    int rc = allowed_rows[c_row];
                    for (int col = 0; col < COLS; ++col) {
                        unsigned char vb = row_digits[allowed_rows[b_row]][col];
                        if (vb < row_digits[allowed_rows[a_row]][col] &&
                            vb < row_digits[rc][col]) {
                            bad = true; break;
                        }
                    }
                    if (!bad) {
                        next_state[idx][out_cnt++] = (uint16_t)(b_row * ALLOWED + c_row);
                    }
                }
                next_cnt[idx] = out_cnt;
            }
        }

        built = true;
    }

    if (n == 0) return (seq_t)0;  /* below the offset – not used */

    /* initialise dp_cur for the first two rows */
    for (int i = 0; i < STATE_CNT; ++i) dp_cur[i] = 0;
    for (int a = 0; a < ALLOWED; ++a) {
        for (int b = 0; b < ALLOWED; ++b) {
            if (top_ok[a][b]) dp_cur[a * ALLOWED + b] = (seq_t)1;
        }
    }

    /* number of additional rows after the first two */
    size_t steps = (n > 1) ? (size_t)(n - 1) : 0;

    for (size_t step = 0; step < steps; ++step) {
        /* zero dp_next */
        for (int i = 0; i < STATE_CNT; ++i) dp_next[i] = 0;
        /* perform transition */
        for (int idx = 0; idx < STATE_CNT; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            unsigned char out_cnt = next_cnt[idx];
            const uint16_t *dest = next_state[idx];
            for (unsigned char k = 0; k < out_cnt; ++k) {
                dp_next[dest[k]] += val;
            }
        }
        /* swap layers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* compute final answer: last row must be not smaller than row before it */
    seq_t ans = (seq_t)0;
    for (int a = 0; a < ALLOWED; ++a) {
        for (int b = 0; b < ALLOWED; ++b) {
            if (top_ok[b][a]) {          /* bottom row >= previous row */
                ans += dp_cur[a * ALLOWED + b];
            }
        }
    }

    return ans;
}
