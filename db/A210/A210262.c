/*
 * A210262  Number of (n+1)X4 0..3 arrays containing all values 0..3 with
 *          every 2X2 subblock having three distinct values, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A210262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A210262 via row‑by‑row dynamic programming.
   States encode the previous row pattern (4 cells, base‑4 digit
   0..3) and the current maximum value seen so far (0..3).
   Transitions enforce the “exactly three distinct values in every
   2×2 subblock” rule and the row‑major introduction order of
   new values. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PATTERN_COUNT 256
#define STATE_COUNT  (PATTERN_COUNT * 4)

static unsigned char pattern_digits[PATTERN_COUNT][4];

/* popcount for 4‑bit masks */
static const unsigned char popcnt4[16] =
{ 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4 };

/* adjacency: each 2×2 subblock of a consecutive pair of rows
   must contain exactly 3 distinct symbols. */
static bool adj_matrix[PATTERN_COUNT][PATTERN_COUNT];

/* Row validity and resulting maximum value after processing a row,
   given a starting maximum (start_max = -1,0,1,2,3). */
static bool row_valid[5][PATTERN_COUNT];
static signed char row_end_max[5][PATTERN_COUNT];

/* Transition lists: for each starting max (0..3) and current pattern,
   list of next patterns that are admissible. */
static uint16_t trans_list[5][PATTERN_COUNT][PATTERN_COUNT];
static uint16_t trans_count[5][PATTERN_COUNT];

static bool tables_built = false;

static void init_tables(void)
{
    if (tables_built) return;

    /* extract base‑4 digits of each pattern */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        int tmp = p;
        for (int j = 3; j >= 0; --j) {
            pattern_digits[p][j] = (unsigned char)(tmp & 3);
            tmp >>= 2;
        }
    }

    /* adjacency matrix */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        for (int q = 0; q < PATTERN_COUNT; ++q) {
            bool ok = true;
            for (int c = 0; c < 3; ++c) {
                unsigned char mask =
                    (1u << pattern_digits[p][c]) |
                    (1u << pattern_digits[p][c+1]) |
                    (1u << pattern_digits[q][c]) |
                    (1u << pattern_digits[q][c+1]);
                if (popcnt4[mask] != 3) { ok = false; break; }
            }
            adj_matrix[p][q] = ok;
        }
    }

    /* row validity & resulting max */
    for (int sm_idx = 0; sm_idx < 5; ++sm_idx) {
        int start_max = sm_idx - 1;   /* -1,0,1,2,3 */
        for (int p = 0; p < PATTERN_COUNT; ++p) {
            int cur_max = start_max;
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                int v = pattern_digits[p][j];
                if (v > cur_max + 1) { ok = false; break; }
                if (v == cur_max + 1) cur_max = v;
            }
            row_valid[sm_idx][p] = ok;
            row_end_max[sm_idx][p] = (signed char)cur_max;
        }
    }

    /* transition lists */
    for (int sm_idx = 0; sm_idx < 5; ++sm_idx) {
        for (int p = 0; p < PATTERN_COUNT; ++p) {
            uint16_t cnt = 0;
            for (int q = 0; q < PATTERN_COUNT; ++q) {
                if (adj_matrix[p][q] && row_valid[sm_idx][q]) {
                    trans_list[sm_idx][p][cnt++] = (uint16_t)q;
                }
            }
            trans_count[sm_idx][p] = cnt;
        }
    }

    tables_built = true;
}

seq_t a(idx_t n)
{
    init_tables();

    seq_t cur[STATE_COUNT];
    seq_t next[STATE_COUNT];
    for (size_t i = 0; i < STATE_COUNT; ++i) {
        cur[i]  = (seq_t)0;
        next[i] = (seq_t)0;
    }

    /* first row: start with max = -1 (index 0) */
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        if (!row_valid[0][p]) continue;
        int m1 = row_end_max[0][p];
        cur[p * 4 + m1] += (seq_t)1;
    }

    size_t rows = (size_t)n + 1;   /* total rows */
    for (size_t step = 1; step < rows; ++step) {
        /* zero next array */
        for (size_t i = 0; i < STATE_COUNT; ++i) next[i] = (seq_t)0;

        /* transition */
        for (size_t idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t val = cur[idx];
            if (val == (seq_t)0) continue;
            int p  = (int)(idx / 4);
            int m  = (int)(idx % 4);
            uint16_t cnt = trans_count[m+1][p];
            for (uint16_t t = 0; t < cnt; ++t) {
                int q   = trans_list[m+1][p][t];
                int mnew = row_end_max[m+1][q];
                next[q * 4 + mnew] += val;
            }
        }
        /* swap arrays for next iteration */
        for (size_t i = 0; i < STATE_COUNT; ++i) cur[i] = next[i];
    }

    /* sum over final states with max == 3 */
    seq_t result = (seq_t)0;
    for (int p = 0; p < PATTERN_COUNT; ++p) {
        result += cur[p * 4 + 3];
    }
    return result;
}
