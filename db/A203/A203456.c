/*
 * A203456  Number of nX4 0..2 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A203456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of n×4 arrays over {0,1,2} where
   each non‑zero element has at least two neighbors
   (horizontal or vertical) with value >= it.
   The algorithm uses dynamic programming over row states. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 81              /* 3^4 possible rows */
#define UP_FLAGS 16          /* 2^4 possible up‑neighbor flags */
#define STATES (ROWS * UP_FLAGS)
#define MAX_TRANS_PER_STATE 81

typedef struct { uint8_t cur; uint8_t new_mask; } transition_t;

/* Precomputed data for every row */
static uint8_t row_val[ROWS][4];          /* values in the row */
static uint8_t lr_sum[ROWS][4];           /* left + right >= counts */

/* Transition tables */
static transition_t TRANS[STATES][MAX_TRANS_PER_STATE];
static uint8_t TRANS_CNT[STATES];

/* Build tables once – executed at program start */
static void precompute_tables(void)
{
    /* encode all rows */
    for (int r = 0; r < ROWS; ++r) {
        int tmp = r;
        for (int j = 0; j < 4; ++j) {
            row_val[r][j] = tmp % 3;
            tmp /= 3;
        }
        for (int j = 0; j < 4; ++j) {
            uint8_t left_ge  = (j > 0   && row_val[r][j-1] >= row_val[r][j]) ? 1 : 0;
            uint8_t right_ge = (j < 3   && row_val[r][j+1] >= row_val[r][j]) ? 1 : 0;
            lr_sum[r][j] = left_ge + right_ge;
        }
    }

    /* build transition lists */
    for (int prev_row = 0; prev_row < ROWS; ++prev_row) {
        for (int up_mask = 0; up_mask < UP_FLAGS; ++up_mask) {
            int s = prev_row * UP_FLAGS + up_mask;
            uint8_t *cnt = &TRANS_CNT[s];
            *cnt = 0;
            for (int cur_row = 0; cur_row < ROWS; ++cur_row) {
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    if (row_val[prev_row][j] == 0) continue; /* zeros are free */
                    int sat = lr_sum[prev_row][j];
                    sat += (up_mask >> j) & 1;
                    sat += (row_val[cur_row][j] >= row_val[prev_row][j]) ? 1 : 0;
                    if (sat < 2) ok = false;
                }
                if (!ok) continue;

                /* next state's up flag */
                uint8_t new_mask = 0;
                for (int j = 0; j < 4; ++j)
                    if (row_val[prev_row][j] >= row_val[cur_row][j]) new_mask |= (1U << j);

                TRANS[s][*cnt].cur = (uint8_t)cur_row;
                TRANS[s][*cnt].new_mask = new_mask;
                ++(*cnt);
            }
        }
    }
}

/* DP arrays – static to keep them in static memory */
static bool tables_built = false;
static seq_t *dp_cur;
static seq_t *dp_next;

/* Return the n-th term of A203456 */
seq_t a(idx_t n)
{
    if (!tables_built) {
        precompute_tables();
        dp_cur  = (seq_t*)calloc(STATES, sizeof(seq_t));
        dp_next = (seq_t*)calloc(STATES, sizeof(seq_t));
        tables_built = true;
    }

    /* Initialise for first row */
    memset(dp_cur, 0, STATES * sizeof(seq_t));
    for (int r = 0; r < ROWS; ++r)
        dp_cur[r * UP_FLAGS] = 1;   /* up_mask = 0 */

    /* Process subsequent rows */
    for (idx_t row = 2; row <= n; ++row) {
        memset(dp_next, 0, STATES * sizeof(seq_t));
        for (int s = 0; s < STATES; ++s) {
            if (dp_cur[s] == 0) continue;
            uint8_t cnt = TRANS_CNT[s];
            for (int t = 0; t < cnt; ++t) {
                int ns = TRANS[s][t].cur * UP_FLAGS + TRANS[s][t].new_mask;
                dp_next[ns] += dp_cur[s];
            }
        }
        /* swap */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
    }

    /* Finalisation: check constraints for the last row (no down neighbor) */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s) {
        if (dp_cur[s] == 0) continue;
        int last_row = s / UP_FLAGS;
        int up_mask = s % UP_FLAGS;
        bool ok = true;
        for (int j = 0; j < 4 && ok; ++j) {
            if (row_val[last_row][j] == 0) continue; /* no constraint for zero */
            int sat = lr_sum[last_row][j] + ((up_mask >> j) & 1);
            if (sat < 2) ok = false;
        }
        if (ok) result += dp_cur[s];
    }
    return result;
}
