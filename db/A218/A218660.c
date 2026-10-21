/*
 * A218660  Number of n X 5 binary arrays with every 0 a king-move neighbor to
 *          some 1.
 * https://oeis.org/A218660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n×5 binary matrices in which every 0 has at
 *            least one king‑move neighbor that is a 1. Computation uses a
 *            transfer‑matrix DP over rows; the state consists of the last row
 *            (5 bits) and a mask of zeros that still need a lower neighbor
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   is_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_zero.h"

/* Width of the matrix */
#define WIDTH 5
/* Number of possible row patterns */
#define PAT  (1u << WIDTH)
/* Total number of states: pattern × needed‑mask */
#define STATES (PAT * PAT)

/* Precomputed helper tables */
static int init_done = 0;
static int mask_down[WIDTH];                     /* columns that can satisfy a previous zero */
static int valid_patterns[PAT][PAT];              /* valid next patterns for each needed‑mask */
static int valid_cnt[PAT];                       /* count of valid patterns for each needed‑mask */
static seq_t needed_mat[PAT][PAT];                /* next needed‑mask from (prev pattern, cur pattern) */

/* Initialization of tables, executed once */
static void init_tables(void) {
    if (init_done) return;
    init_done = 1;

    /* mask_down[j] – bitmask of columns that a 1 in that column
       can satisfy a zero in column j of the previous row */
    for (int j = 0; j < WIDTH; ++j) {
        int m = 1 << j;
        if (j > 0) m |= 1 << (j - 1);
        if (j < WIDTH - 1) m |= 1 << (j + 1);
        mask_down[j] = m;
    }

    /* Build valid_patterns and valid_cnt for each needed‑mask M */
    for (int M = 0; M < PAT; ++M) {
        int cnt = 0;
        for (int p = 0; p < PAT; ++p) {
            int ok = 1;
            for (int j = 0; j < WIDTH; ++j)
                if (M & (1 << j))
                    if ((p & mask_down[j]) == 0) { ok = 0; break; }
            if (ok) {
                valid_patterns[M][cnt++] = p;
            }
        }
        valid_cnt[M] = cnt;
    }

    /* Build needed_mat[p_prev][p_cur] */
    for (int pp = 0; pp < PAT; ++pp) {
        for (int pc = 0; pc < PAT; ++pc) {
            int nm = 0;
            for (int j = 0; j < WIDTH; ++j) {
                if (pc & (1 << j)) continue;          /* cell is 1, nothing to satisfy */
                int sat = 0;
                if (j > 0 && (pc & (1 << (j - 1)))) sat = 1;
                if (j < WIDTH - 1 && (pc & (1 << (j + 1)))) sat = 1;
                if (pp & (1 << j)) sat = 1;
                if (j > 0 && (pp & (1 << (j - 1)))) sat = 1;
                if (j < WIDTH - 1 && (pp & (1 << (j + 1)))) sat = 1;
                if (!sat) nm |= 1 << j;
            }
            needed_mat[pp][pc] = (seq_t)nm;
        }
    }
}

/* Main function */
seq_t a(idx_t n) {
    init_tables();

    if (n == 0) return 0;          /* undefined offset, but safe */

    /* dp arrays: counts for each state (pattern, needed‑mask) */
    seq_t *dp_cur = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *dp_nxt = (seq_t *)calloc(STATES, sizeof(seq_t));

    /* Initialization for the first row (no upper neighbours) */
    for (int p = 0; p < PAT; ++p) {
        int nm = (int)needed_mat[0][p];
        dp_cur[(p << WIDTH) | nm] = 1;
    }

    /* Process rows 2..n */
    for (size_t row = 1; row < (size_t)n; ++row) {
        /* zero next array */
        for (int i = 0; i < STATES; ++i) dp_nxt[i] = 0;

        /* transition */
        for (int s = 0; s < STATES; ++s) {
            if (is_zero(dp_cur[s])) continue;
            seq_t val = dp_cur[s];
            int pp = s >> WIDTH;
            int M =  s & (PAT - 1);

            int *list = valid_patterns[M];
            int listlen = valid_cnt[M];

            for (int k = 0; k < listlen; ++k) {
                int pc = list[k];
                int nm = (int)needed_mat[pp][pc];
                int ns = (pc << WIDTH) | nm;
                dp_nxt[ns] += val;
            }
        }

        /* swap arrays */
        seq_t *tmp = dp_cur; dp_cur = dp_nxt; dp_nxt = tmp;
    }

    /* Sum states where the final needed‑mask is zero */
    seq_t ans = 0;
    for (int s = 0; s < STATES; ++s) {
        if ((s & (PAT - 1)) == 0) ans += dp_cur[s];
    }

    free(dp_cur);
    free(dp_nxt);
    return ans;
}
