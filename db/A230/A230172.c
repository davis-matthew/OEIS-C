/*
 * A230172  Number of n X 4 0..2 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value 2-x(i,j).
 * https://oeis.org/A230172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230172: number of n×4 arrays over {0,1,2}
   such that each entry has at least one neighbour (horizontal,
   vertical, diagonal, or antidiagonal) of opposite value.
   Computation is performed by a DP over rows, using the 3⁴=81 possible
   row patterns and the 2⁴=16 possible “need” masks.  The algorithm
   runs in O(n) time and uses only local arrays of fixed size. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 3⁴ row patterns, 2⁴ possible need masks */
enum { PATTERNS = 81, MASKS = 16, STATES = 81 * 16 };

/* Pre‑computed data that depends only on the row patterns. */
static int  pattern_val[PATTERNS][4];          /* digits of a pattern */
static uint8_t horiz_need[PATTERNS];          /* columns that need a neighbour */
static uint8_t sat_mask[PATTERNS][PATTERNS];   /* columns of previous row
                                                satisfied by current row   */
static uint8_t next_need[PATTERNS][PATTERNS];  /* columns that still need
                                                a neighbour in current row */
static bool tables_built = false;

/* Build tables only once. */
static void init_tables(void)
{
    if (tables_built) return;
    tables_built = true;

    /* Enumerate all 3⁴ patterns. */
    for (int p = 0; p < PATTERNS; ++p) {
        int x = p;
        for (int col = 0; col < 4; ++col) {
            pattern_val[p][col] = x % 3;
            x /= 3;
        }

        /* Determine horizontal need for this pattern. */
        uint8_t need = 0;
        for (int col = 0; col < 4; ++col) {
            int v = pattern_val[p][col];
            int tgt = 2 - v;
            bool ok = false;
            if (col > 0   && pattern_val[p][col-1] == tgt) ok = true;
            if (col < 3   && pattern_val[p][col+1] == tgt) ok = true;
            if (!ok) need |= (1U << col);
        }
        horiz_need[p] = need;
    }

    /* Pre‑compute transitions for every pair of rows. */
    for (int prev = 0; prev < PATTERNS; ++prev) {
        for (int cur = 0; cur < PATTERNS; ++cur) {
            uint8_t sat      = 0;
            uint8_t need_col = 0;
            for (int col = 0; col < 4; ++col) {
                int prev_v = pattern_val[prev][col];
                int prev_tgt = 2 - prev_v;

                /* Does current row satisfy this cell of previous row? */
                bool sat_col = false;
                if (col > 0   && pattern_val[cur][col-1] == prev_tgt) sat_col = true;
                if (pattern_val[cur][col]   == prev_tgt)                sat_col = true;
                if (col < 3   && pattern_val[cur][col+1] == prev_tgt) sat_col = true;
                if (sat_col) sat |= (1U << col);

                /* Determine need for current cell after horizontal and vertical checks. */
                int cur_v  = pattern_val[cur][col];
                int cur_tgt= 2 - cur_v;
                bool ok = false;
                if (col > 0   && pattern_val[cur][col-1] == cur_tgt) ok = true;
                if (col < 3   && pattern_val[cur][col+1] == cur_tgt) ok = true;
                if (!ok) {   /* try vertical/diagonal from previous row */
                    if (col > 0   && pattern_val[prev][col-1] == cur_tgt) ok = true;
                    if (pattern_val[prev][col]   == cur_tgt)               ok = true;
                    if (col < 3   && pattern_val[prev][col+1] == cur_tgt) ok = true;
                }
                if (!ok) need_col |= (1U << col);
            }
            sat_mask[prev][cur]   = sat;
            next_need[prev][cur]  = need_col;
        }
    }
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_tables();

    /* DP arrays for the current and next row.   The size is small enough
       to keep them on the stack. */
    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];
    for (int i = 0; i < STATES; ++i) dp_cur[i] = 0;

    /* Initial row: any pattern with its horizontal need mask. */
    for (int p = 0; p < PATTERNS; ++p) {
        int state = (p << 4) | horiz_need[p];
        dp_cur[state] = 1;
    }

    /* Process each subsequent row. */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        /* For each previous pattern. */
        for (int prev = 0; prev < PATTERNS; ++prev) {
            /* Pre‑compute subset sums of counts for this previous pattern. */
            seq_t subset_sum[MASKS];
            for (int m = 0; m < MASKS; ++m) subset_sum[m] = 0;

            for (int m = 0; m < MASKS; ++m) {
                seq_t val = dp_cur[(prev << 4) | m];
                if (val == 0) continue;
                /* Add val to all superset masks of m. */
                for (int sup = m; sup < MASKS; ++sup) {
                    if ((sup & m) == m) subset_sum[sup] += val;
                }
            }

            /* Try all current rows. */
            for (int cur = 0; cur < PATTERNS; ++cur) {
                uint8_t sat  = sat_mask[prev][cur];
                uint8_t need = next_need[prev][cur];
                seq_t add = subset_sum[sat];
                if (add == 0) continue;
                dp_next[(cur << 4) | need] += add;
            }
        }
        /* Swap pointers for next iteration. */
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all states of the last row whose need mask is zero. */
    seq_t result = 0;
    for (int p = 0; p < PATTERNS; ++p) {
        result += dp_cur[(p << 4) | 0];
    }
    return result;
}
