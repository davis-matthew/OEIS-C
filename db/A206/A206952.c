/*
 * A206952  Number of nX3 0..2 arrays avoiding the pattern z z+1 z in any row,
 *          column or nw-to-se diagonal.
 * https://oeis.org/A206952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×3 arrays over {0,1,2} that avoid the
 * pattern z z+1 z in any row, column or NW‑to‑SE diagonal.
 * Computed by dynamic programming on pairs of consecutive rows.
 */
#include <stdint.h>
#include <stdbool.h>

#define ROWS 25          /* number of rows that don't contain a horizontal pattern */
#define STATES (ROWS * ROWS)   /* number of automaton states = pair of rows */

/* Precomputed transition tables */
static uint16_t  next_states[STATES][ROWS];
static unsigned char  next_count[STATES];
static unsigned char  row_digits[ROWS][3];

/* Build tables on first use */
static void init_tables(void)
{
    /* Enumerate all 3‑digit rows and keep the 25 that avoid a horizontal pattern. */
    int idx = 0;
    for (int v = 0; v < 27; ++v) {
        int d0 = v / 9;
        int d1 = (v / 3) % 3;
        int d2 = v % 3;
        if (!((d0 == 0 && d1 == 1 && d2 == 0) ||
              (d0 == 1 && d1 == 2 && d2 == 1))) {
            row_digits[idx][0] = (unsigned char)d0;
            row_digits[idx][1] = (unsigned char)d1;
            row_digits[idx][2] = (unsigned char)d2;
            ++idx;
        }
    }

    /* Transition table: from state (a,b) we may go to (b,c) if vertical and diagonal
     * constraints are satisfied. */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            int cur = a * ROWS + b;
            unsigned char cnt = 0;
            for (int c = 0; c < ROWS; ++c) {
                bool ok = true;
                /* Vertical constraints: for each column */
                for (int col = 0; col < 3; ++col) {
                    int va = row_digits[a][col];
                    int vb = row_digits[b][col];
                    int vc = row_digits[c][col];
                    if ((va == 0 && vb == 1 && vc == 0) ||
                        (va == 1 && vb == 2 && vc == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;
                /* Diagonal constraint: cells (a,0),(b,1),(c,2) */
                if ((row_digits[a][0] == 0 && row_digits[b][1] == 1 &&
                     row_digits[c][2] == 0) ||
                    (row_digits[a][0] == 1 && row_digits[b][1] == 2 &&
                     row_digits[c][2] == 1)) {
                    continue;
                }
                /* Valid transition */
                next_states[cur][cnt] = (uint16_t)(b * ROWS + c);
                ++cnt;
            }
            next_count[cur] = cnt;
        }
    }
}

/* DP buffers */
static seq_t dp_buf[STATES];
static seq_t next_buf[STATES];
static bool initialized = false;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined below offset */

    if (!initialized) {
        init_tables();
        initialized = true;
    }

    if (n == 1) return (seq_t)25;          /* single row */
    if (n == 2) return (seq_t)25 * (seq_t)25;   /* two independent rows */

    /* Initialise DP for the first two rows: every pair of allowed rows is valid. */
    for (int i = 0; i < STATES; ++i)
        dp_buf[i] = (seq_t)1;

    /* Process rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero next buffer */
        for (int i = 0; i < STATES; ++i)
            next_buf[i] = (seq_t)0;

        /* Transition from each state */
        for (int s = 0; s < STATES; ++s) {
            seq_t cur_val = dp_buf[s];
            if (!cur_val) continue;
            unsigned char cnt = next_count[s];
            for (int k = 0; k < cnt; ++k) {
                uint16_t ns = next_states[s][k];
                next_buf[ns] += cur_val;
            }
        }

        /* Copy back for next iteration */
        for (int i = 0; i < STATES; ++i)
            dp_buf[i] = next_buf[i];
    }

    /* Sum over all final states to get total number of arrays. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATES; ++i)
        result += dp_buf[i];
    return result;
}
