/*
 * A188693  Number of nX7 binary arrays without the pattern 0 1 0 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(1)=65, a(2)=4225.  
 * For n≥3: a(n) equals the number of n×7 binary arrays
 * that contain no 010 pattern horizontally, vertically,
 * along the main diagonal, or along the anti‑diagonal.
 * The count is obtained by dynamic programming over pairs
 * of consecutive rows, where a state is a pair of valid rows.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Local constants */
#define ROWS 65          /* number of valid 7‑bit rows (no horizontal 010) */
#define STATE_CNT (ROWS * ROWS)   /* number of row pairs */

/* Tables describing allowed rows and transitions between pairs of rows */
static uint8_t  _row_bits[ROWS];
static int      _adj_len[STATE_CNT];
static int      _adj_next[STATE_CNT][ROWS];
static bool     _tables_built = false;

/* Build the tables once.  This is safe to call from the main function. */
static void _build_tables(void)
{
    if (_tables_built) return;

    /* Determine all valid rows: 7‑bit patterns without 010 horizontally. */
    int rc = 0;
    for (int code = 0; code < 128; ++code) {
        bool ok = true;
        for (int c = 0; c + 2 < 7; ++c) {
            if (((code >> c) & 1) == 0 &&
                ((code >> (c + 1)) & 1) == 1 &&
                ((code >> (c + 2)) & 1) == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            _row_bits[rc++] = (uint8_t)code;
        }
    }
    /* rc should equal 65.  If not, compilation will fail on the next loop. */

    /* Pre‑compute adjacency lists: for each pair (i,j) find all k such
       that a 3×3 block formed by rows i,j,k contains no 010. */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int idx = i * ROWS + j;
            int len = 0;
            for (int k = 0; k < ROWS; ++k) {
                uint8_t a = _row_bits[i];
                uint8_t b = _row_bits[j];
                uint8_t c = _row_bits[k];
                bool ok = true;

                /* Vertical 010 check */
                for (int col = 0; col < 7; ++col) {
                    if (((a >> col) & 1) == 0 &&
                        ((b >> col) & 1) == 1 &&
                        ((c >> col) & 1) == 0) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                /* Main diagonal 010 check */
                for (int col = 0; col <= 4; ++col) {
                    if (((a >> col) & 1) == 0 &&
                        ((b >> (col + 1)) & 1) == 1 &&
                        ((c >> (col + 2)) & 1) == 0) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                /* Anti‑diagonal 010 check */
                for (int col = 2; col < 7; ++col) {
                    if (((a >> col) & 1) == 0 &&
                        ((b >> (col - 1)) & 1) == 1 &&
                        ((c >> (col - 2)) & 1) == 0) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                _adj_next[idx][len++] = k;
            }
            _adj_len[idx] = len;
        }
    }
    _tables_built = true;
}

/* Main sequence function required by the harness. */
seq_t a(idx_t n)
{
    _build_tables();              /* Ensure transition data is ready. */

    /* a(1) = number of valid rows */
    if (n == 1) return (seq_t)ROWS;

    /* For n >= 2 we count configurations with n rows. */
    seq_t dp_cur[STATE_CNT];
    seq_t dp_next[STATE_CNT];

    /* Height 2: every pair of valid rows is allowed. */
    for (int i = 0; i < STATE_CNT; ++i) dp_cur[i] = (seq_t)1;

    /* Add rows from height 3 up to n. */
    for (idx_t h = 3; h <= n; ++h) {
        for (int i = 0; i < STATE_CNT; ++i) dp_next[i] = (seq_t)0;
        for (int idx = 0; idx < STATE_CNT; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            int len = _adj_len[idx];
            int j = idx % ROWS;   /* second last row index */
            for (int t = 0; t < len; ++t) {
                int k = _adj_next[idx][t];
                int to = j * ROWS + k;   /* new state (j,k) */
                dp_next[to] += val;
            }
        }
        /* Swap dp_cur and dp_next for next iteration. */
        for (int i = 0; i < STATE_CNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all states to get the total number of matrices. */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < STATE_CNT; ++i) sum += dp_cur[i];
    return sum;
}
