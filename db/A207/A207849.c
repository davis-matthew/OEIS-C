/*
 * A207849  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207849:
 * a(1) = 26   (valid 6‑bit columns)
 * a(2) = 26²
 * For n ≥ 3 the number of 6×n binary arrays avoiding
 *  horizontal “000” and “001” (i.e. no two consecutive zeros in any row)
 *  and vertical “000” and “111” (no three consecutive equal bits in any column)
 * is computed by a simple dynamic programming on pairs of columns.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ----------  constants and static tables  ---------- */
#define MAX_COL 26          /* number of valid 6‑bit columns */

/* valid columns (list of 6‑bit masks that avoid 000 or 111 vertically) */
static uint8_t valid_col[MAX_COL];
static int      valid_cnt = 0;

/* for each column j, a bitmask of columns i that can precede j
 * (i.e. no row r has both i and j zero).                           */
static uint32_t mask_prev[MAX_COL];

/* DP working arrays */
static seq_t dp1[MAX_COL][MAX_COL];
static seq_t dp2[MAX_COL][MAX_COL];

/* flag signalling that the tables are already initialized */
static bool init_done = false;

/* ----------  helper functions  ---------- */

/* check whether a 6‑bit column has a vertical 000 or 111 */
static inline bool col_valid(uint8_t col)
{
    for (int r = 0; r <= 3; ++r) {            /* 4 triples of 3 consecutive rows */
        int b0 = (col >> r)     & 1u;
        int b1 = (col >> (r + 1)) & 1u;
        int b2 = (col >> (r + 2)) & 1u;
        if (b0 == 0 && b1 == 0 && b2 == 0) return false;
        if (b0 == 1 && b1 == 1 && b2 == 1) return false;
    }
    return true;
}

/* horizontal rule: for each row, the pair of bits from c1 and c2 must not
 * both be 0 (otherwise the triple with a third column would be 000 or 001). */
static inline bool horiz_ok(uint8_t c1, uint8_t c2)
{
    for (int r = 0; r < 6; ++r) {
        if ( ((c1 >> r) & 1u) == 0 && ((c2 >> r) & 1u) == 0 )
            return false;
    }
    return true;
}

/* initialise the tables – run once before any call to a() */
static void init_tables(void)
{
    /* build list of valid columns */
    valid_cnt = 0;
    for (uint8_t m = 0; m < 64; ++m) {
        if (col_valid(m)) {
            valid_col[valid_cnt++] = m;
        }
    }

    /* build mask_prev: for each column j, bit i is set iff i can precede j */
    for (int j = 0; j < valid_cnt; ++j) {
        uint32_t mask = 0;
        for (int i = 0; i < valid_cnt; ++i) {
            if (horiz_ok(valid_col[i], valid_col[j]))
                mask |= (1u << i);
        }
        mask_prev[j] = mask;
    }

    init_done = true;
}

/* ----------  main sequence function  ---------- */

seq_t a(idx_t n)
{
    if (!init_done)
        init_tables();

    if (n == 0) return (seq_t)0;       /* undefined, but safe return */
    if (n == 1) return (seq_t)valid_cnt;
    if (n == 2) return (seq_t)valid_cnt * (seq_t)valid_cnt;

    /* DP: cur holds DP for width (step – 1),
       next will hold DP for width step
       dp dimension: [previous][current] */
    seq_t (*cur)[MAX_COL]  = dp1;
    seq_t (*next)[MAX_COL] = dp2;

    /* initialise to width 2: every pair contributes 1 */
    for (int i = 0; i < valid_cnt; ++i)
        for (int j = 0; j < valid_cnt; ++j)
            cur[i][j] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        /* fill next from cur */
        for (int j = 0; j < valid_cnt; ++j) {
            seq_t sum = 0;
            uint32_t mask = mask_prev[j];
            /* sum over all i that can precede j */
            for (int i = 0; i < valid_cnt; ++i)
                if (mask & (1u << i))
                    sum += cur[i][j];
            /* all k are allowed as the new column */
            for (int k = 0; k < valid_cnt; ++k)
                next[j][k] = sum;
        }
        /* prepare for next iteration */
        seq_t (*tmp)[MAX_COL] = cur;
        cur = next;
        next = tmp;
    }

    /* sum all states for the final width */
    seq_t total = 0;
    for (int i = 0; i < valid_cnt; ++i)
        for (int j = 0; j < valid_cnt; ++j)
            total += cur[i][j];

    return total;
}
