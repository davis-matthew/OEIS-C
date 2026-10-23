/*
 * A183515  Number of nX7 binary arrays with each element equal to either the
 *          sum mod 2 of its horizontal and vertical neighbors or the sum mod
 *          2 of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×7 binary arrays such that each element equals
 * either the sum mod 2 of its horizontal and vertical neighbors, or the
 * sum mod 2 of its diagonal and antidiagonal neighbors.
 *
 * The implementation uses a transfer‑matrix style dynamic programming over
 * consecutive pairs of rows.  Each state is a pair of 7‑bit patterns
 * (previous row, current row).  For every such state we pre‑compute the
 * list of next rows that make the middle row satisfy the local condition.
 * The DP then walks n − 1 transitions; after processing the last row we
 * check the final boundary condition with a zero row.
 */
#include <stdint.h>
#include <stdbool.h>

#define WIDTH      7
#define ROWS       128          /* 2^7 binary rows */
#define STATES     (ROWS * ROWS) /* pair of rows */

/* adjacency information ---------------------------------------------------- */

static uint8_t adj_next[STATES][ROWS]; /* next rows for each (prev,curr) state   */
static uint8_t adj_count[STATES];      /* number of next rows for each state     */
static bool adj_ready = false;

/* check whether a triple of rows satisfies the local condition for the
 * middle row.  All indices are 0‑based and rows are 7‑bit patterns.       */
static bool
triple_valid(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int j = 0; j < WIDTH; ++j) {
        uint8_t curr_bit = (curr >> j) & 1;

        uint8_t up       = (prev >> j) & 1;
        uint8_t down     = (next >> j) & 1;
        uint8_t left     = (j > 0)      ? ((curr >> (j-1)) & 1) : 0;
        uint8_t right    = (j+1 < WIDTH) ? ((curr >> (j+1)) & 1) : 0;
        uint8_t up_left  = (j > 0)      ? ((prev >> (j-1)) & 1) : 0;
        uint8_t up_right = (j+1 < WIDTH) ? ((prev >> (j+1)) & 1) : 0;
        uint8_t down_left  = (j > 0)      ? ((next >> (j-1)) & 1) : 0;
        uint8_t down_right = (j+1 < WIDTH) ? ((next >> (j+1)) & 1) : 0;

        uint8_t s1 = up ^ down ^ left ^ right;                 /* horiz+vert */
        uint8_t s2 = up_left ^ up_right ^ down_left ^ down_right; /* diag+anti */

        if (curr_bit != s1 && curr_bit != s2)
            return false;
    }
    return true;
}

/* initialise adjacency tables once ------------------------------------------------ */
static void init_adjacency(void)
{
    for (uint16_t prev = 0; prev < ROWS; ++prev) {
        for (uint16_t curr = 0; curr < ROWS; ++curr) {
            uint16_t idx = prev * ROWS + curr;
            uint8_t count = 0;
            for (uint16_t next = 0; next < ROWS; ++next) {
                if (triple_valid((uint8_t)prev,
                                 (uint8_t)curr,
                                 (uint8_t)next))
                    adj_next[idx][count++] = (uint8_t)next;
            }
            adj_count[idx] = count;
        }
    }
    adj_ready = true;
}

/* DP arrays -------------------------------------------------------------- */
static seq_t dp[STATES];
static seq_t newdp[STATES];

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!adj_ready) init_adjacency();

    if (n == 0) return 0;    /* not part of the sequence, but harmless */

    /* Special case n = 1: only the boundary condition with zero rows is needed */
    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t r = 0; r < ROWS; ++r)
            if (triple_valid(0, r, 0))
                ans += 1;
        return ans;
    }

    /* initialise dp for state (prev = 0, curr = r) */
    for (int i = 0; i < STATES; ++i) dp[i] = 0;
    for (uint8_t r = 0; r < ROWS; ++r)
        dp[r] = 1;  /* state index 0*ROWS + r */

    /* perform n-1 transitions */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i) newdp[i] = 0;

        for (int idx = 0; idx < STATES; ++idx) {
            seq_t ways = dp[idx];
            if (ways == 0) continue;
            uint8_t curr = (uint8_t)(idx % ROWS);
            uint8_t outc = adj_count[idx];
            for (uint8_t k = 0; k < outc; ++k) {
                uint8_t next = adj_next[idx][k];
                int newidx = (int)curr * ROWS + next;
                newdp[newidx] += ways;
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < STATES; ++i) dp[i] = newdp[i];
    }

    /* final boundary check with next row = 0 */
    seq_t ans = 0;
    for (int idx = 0; idx < STATES; ++idx) {
        seq_t ways = dp[idx];
        if (ways == 0) continue;
        uint8_t prev = (uint8_t)(idx / ROWS);
        uint8_t curr = (uint8_t)(idx % ROWS);
        if (triple_valid(prev, curr, 0))
            ans += ways;
    }
    return ans;
}
