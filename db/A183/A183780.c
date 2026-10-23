/*
 * A183780  Half the number of (n+1) X 8 binary arrays with no 2 X 2 subblock
 *          having exactly 2 ones.
 * https://oeis.org/A183780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = half the number of (n+1) × 8 binary arrays with no * 2 ×
 *            2 subblock having exactly 2 ones
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define N_ROWS 256          /* number of possible 8‑bit rows */
#define MAX_OUT 256         /* maximum outdegree (≤ N_ROWS) */

/* adjacency information, built once */
static bool adj_inited = false;
static uint16_t adj_list[N_ROWS][MAX_OUT];
static uint8_t  adj_cnt[N_ROWS];

/* row dynamic programming tables */
static seq_t row_prev[N_ROWS];
static seq_t row_next[N_ROWS];

/* --- helper: check compatibility of two rows -------------------- */
static inline bool rows_compatible(uint8_t a, uint8_t b)
{
    for (int j = 0; j < 7; ++j) {
        int ones = ((a >> j) & 1) + ((a >> (j + 1)) & 1) +
                   ((b >> j) & 1) + ((b >> (j + 1)) & 1);
        if (ones == 2) return false;     /* forbidden 2×2 block */
    }
    return true;
}

/* --- initialise adjacency lists ------------------------------ */
static void init_adjacency(void)
{
    for (int u = 0; u < N_ROWS; ++u) {
        uint8_t a = (uint8_t)u;
        uint8_t cnt = 0;
        for (int v = 0; v < N_ROWS; ++v) {
            if (rows_compatible(a, (uint8_t)v)) {
                adj_list[u][cnt++] = (uint16_t)v;
            }
        }
        adj_cnt[u] = cnt;
    }
    adj_inited = true;
}

/* --- main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    /* base case: n = 0 → (1 row, 8 columns) */
    if (n == 0) return (seq_t)128;

    if (!adj_inited) init_adjacency();

    /* initialise DP: any first row is possible */
    for (int i = 0; i < N_ROWS; ++i) row_prev[i] = (seq_t)1;

    /* iterate n steps (adding n rows after the first) */
    for (idx_t step = 1; step <= n; ++step) {
        /* clear next array */
        for (int i = 0; i < N_ROWS; ++i) row_next[i] = (seq_t)0;

        /* transition */
        for (int u = 0; u < N_ROWS; ++u) {
            seq_t val = row_prev[u];
            if (val == (seq_t)0) continue;
            uint8_t cnt = adj_cnt[u];
            for (uint8_t k = 0; k < cnt; ++k) {
                row_next[adj_list[u][k]] += val;
            }
        }

        /* swap arrays for next step */
        for (int i = 0; i < N_ROWS; ++i) row_prev[i] = row_next[i];
    }

    /* sum over all ending rows */
    seq_t total = (seq_t)0;
    for (int i = 0; i < N_ROWS; ++i) total += row_prev[i];

    /* count each array twice (symmetry), so halve the result */
    return total / ((seq_t)2);
}
