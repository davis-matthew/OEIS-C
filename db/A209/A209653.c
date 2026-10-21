/*
 * A209653  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 1 0 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A209653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n 0/1 arrays avoiding * horizontal patterns 0
 *            0 1 and 1 0 0, vertical patterns * 0 0 1 and 1 0 1. Computed
 *            with a DP over admissible * columns and their pairwise
 *            transitions
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define COLS 12                          /* number of admissible 6‑bit columns */

static uint8_t allowed[COLS];            /* admissible columns, computed once */
static bool initialized = false;

/* Check if a single 6‑bit column satisfies the vertical constraints: */
/* no 0 0 1 or 1 0 1 in any three consecutive rows. */
static bool col_valid(uint8_t col)
{
    for (int r = 0; r <= 3; ++r) {
        int b0 = (col >> r)     & 1;
        int b1 = (col >> (r+1)) & 1;
        int b2 = (col >> (r+2)) & 1;
        if ((b0==0 && b1==0 && b2==1) ||
            (b0==1 && b1==0 && b2==1))
            return false;
    }
    return true;
}

/* Check if three columns form a valid horizontal triple:        */
/* in every row the 3‑bit pattern is neither 0 0 1 nor 1 0 0.       */
static bool triple_valid(uint8_t a, uint8_t b, uint8_t c)
{
    for (int j = 0; j < 6; ++j) {
        int x = (a >> j) & 1;
        int y = (b >> j) & 1;
        int z = (c >> j) & 1;
        if ((x==0 && y==0 && z==1) || (x==1 && y==0 && z==0))
            return false;
    }
    return true;
}

/* Initialize the list of admissible columns.  Called once.        */
static void init_columns(void)
{
    if (initialized) return;
    int cnt = 0;
    for (int col = 0; col < 64; ++col) {
        if (col_valid((uint8_t)col))
            allowed[cnt++] = (uint8_t)col;
    }
    /* cnt must be 12 for this sequence. */
    initialized = true;
}

/* --------------------------------------------------------------- */
/* Public function required by the harness.                          */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_columns();

    /* Very small cases */
    if (n == 1) return (seq_t)COLS;          /* 12 */
    if (n == 2) return (seq_t)COLS * (seq_t)COLS; /* 144 */

    /* Dynamic programming: dp[i][j] = number of ways to end with columns
     * allowed[i] then allowed[j] (i.e., last two columns).              */
    seq_t dp[COLS][COLS];
    seq_t newdp[COLS][COLS];

    /* Initialise dp for length 2: every pair is allowed.                */
    for (int i = 0; i < COLS; ++i)
        for (int j = 0; j < COLS; ++j)
            dp[i][j] = 1;

    /* Process columns 3 … n.                                            */
    for (idx_t step = 3; step <= n; ++step) {
        /* Zero newdp */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                newdp[i][j] = 0;

        for (int x = 0; x < COLS; ++x) {
            for (int y = 0; y < COLS; ++y) {
                seq_t cur = dp[x][y];
                if (cur == 0) continue;
                for (int z = 0; z < COLS; ++z) {
                    if (triple_valid(allowed[x], allowed[y], allowed[z]))
                        newdp[y][z] += cur;
                }
            }
        }

        /* Copy newdp into dp for the next iteration. */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum all ways for the final length n.                                 */
    seq_t result = 0;
    for (int i = 0; i < COLS; ++i)
        for (int j = 0; j < COLS; ++j)
            result += dp[i][j];

    return result;
}
