/*
 * A203282  Number of nX5 binary arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A203282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays in which every element has,
 *            among its horizontal or vertical neighbours, at least one 0 and
 *            at least one 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define COLS 5
#define COL_MASK  (1u << COLS) - 1
#define NEED_BITS 10
#define STATE_MASK ((1u << NEED_BITS) - 1)

/* Pre‑computed tables -----------------------------------------------------*/
static uint16_t need_from[COL_MASK + 1][COL_MASK + 1];
static uint32_t allowed[STATE_MASK + 1];
static uint16_t init_need[COL_MASK + 1];
static bool tables_inited = false;

/* Build lookup tables once. */
static void precompute(void)
{
    int j;
    /* ----------------------------------- */
    /* 1. need_from[prev][curr]  */
    for (int pm = 0; pm <= COL_MASK; ++pm) {
        for (int cm = 0; cm <= COL_MASK; ++cm) {
            uint16_t need0 = 0, need1 = 0;
            for (j = 0; j < COLS; ++j) {
                bool has0 = false, has1 = false;
                /* horizontal neighbours */
                if (j > 0) {
                    int h = (cm >> (j - 1)) & 1;
                    if (h == 0) has0 = true; else has1 = true;
                }
                if (j < COLS - 1) {
                    int h = (cm >> (j + 1)) & 1;
                    if (h == 0) has0 = true; else has1 = true;
                }
                /* vertical neighbour above (from previous row) */
                int pv = (pm >> j) & 1;
                if (pv == 0) has0 = true; else has1 = true;
                /* determine missing type for below neighbour */
                if (! (has0 && has1)) {
                    if (!has0) need0 |= 1u << j;   /* need a 0 below */
                    if (!has1) need1 |= 1u << j;   /* need a 1 below */
                }
            }
            need_from[pm][cm] = (uint16_t)(need0 | (need1 << 5));
        }
    }

    /* ----------------------------------- */
    /* 2. allowed[needState]  */
    for (int s = 0; s <= STATE_MASK; ++s) {
        uint16_t need0 = s & 0x1F;
        uint16_t need1 = s >> 5;
        uint32_t mask = 0;
        for (int cm = 0; cm <= COL_MASK; ++cm) {
            if ( (cm & need0) != 0 ) continue;                 /* needs 0 -> bit must be 0 */
            if ( ((~cm) & COL_MASK & need1) != 0 ) continue;   /* needs 1 -> bit must be 1 */
            mask |= 1u << cm;
        }
        allowed[s] = mask;
    }

    /* ----------------------------------- */
    /* 3. init_need[mask] for the first row (no above neighbour) */
    for (int m = 0; m <= COL_MASK; ++m) {
        uint16_t need0 = 0, need1 = 0;
        for (j = 0; j < COLS; ++j) {
            bool has0 = false, has1 = false;
            /* horizontal neighbours only */
            if (j > 0) {
                int h = (m >> (j - 1)) & 1;
                if (h == 0) has0 = true; else has1 = true;
            }
            if (j < COLS - 1) {
                int h = (m >> (j + 1)) & 1;
                if (h == 0) has0 = true; else has1 = true;
            }
            /* no vertical above */
            if (! (has0 && has1)) {
                if (!has0) need0 |= 1u << j;
                if (!has1) need1 |= 1u << j;
            }
        }
        init_need[m] = (uint16_t)(need0 | (need1 << 5));
    }

    tables_inited = true;
}

/* Main function -----------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Ensure tables are ready */
    if (!tables_inited) precompute();

    if (n == 0) {
        return (seq_t)0;               /* offset is 1, value for n<1 does not matter */
    }

    /* DP arrays ---------------------------------------- */
    static seq_t dp[COL_MASK + 1][STATE_MASK + 1];
    static seq_t ndp[COL_MASK + 1][STATE_MASK + 1];

    /* initialise to zero */
    memset(dp, 0, sizeof(dp));

    /* initial row (row 1) */
    for (int m = 0; m <= COL_MASK; ++m) {
        uint16_t s = init_need[m];
        dp[m][s] += (seq_t)1;
    }

    /* If only one row, answer is sum of states with no pending needs */
    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int m = 0; m <= COL_MASK; ++m) {
            ans += dp[m][0];
        }
        return ans;
    }

    /* Process rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        /* clear ndp */
        memset(ndp, 0, sizeof(ndp));

        for (int pm = 0; pm <= COL_MASK; ++pm) {
            for (int s = 0; s <= STATE_MASK; ++s) {
                seq_t val = dp[pm][s];
                if (val == (seq_t)0) continue;

                uint32_t bm = allowed[s];
                while (bm) {
                    int cm = __builtin_ctz(bm);
                    bm &= bm - 1;                     /* clear lowest set bit */
                    uint16_t new_s = need_from[pm][cm];
                    ndp[cm][new_s] += val;
                }
            }
        }

        /* copy ndp to dp for next iteration */
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Sum over final states where no pending needs */
    seq_t ans = (seq_t)0;
    for (int m = 0; m <= COL_MASK; ++m) {
        ans += dp[m][0];
    }
    return ans;
}
