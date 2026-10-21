/*
 * A236518  Number of (n+1)X(3+1) 0..1 arrays with the difference of the upper
 *          median and minimum value of each 2X2 subblock in lexicographically
 *          nondecreasing order columnwise and nonincreasing rowwise.
 * https://oeis.org/A236518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A236518 via dynamic programming on rows.
   The algorithm iterates over the (n+1) rows of a 0/1 matrix of
   size (n+1)×4. For each pair of consecutive rows the 2×2 blocks
   produce a 3‑bit diff mask: bit j is 1 if the corresponding
   2×2 subblock has 2 or 3 ones (the "difference of the upper median
   and minimum" of that 2×2 subblock).  The allowed diff masks are the
   four non‑increasing patterns 000,100,110,111.  While scanning rows
   we keep one state consisting of the last row and the last diff mask.
   Transition to the next row is allowed only if the new diff mask is a
   superset (bitwise) of the previous one, which guarantees that
   columnwise diff values are nondecreasing.  The total number of
   matrices satisfying the constraints is returned. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Compile‑time constants */
#define ROWS 16        /* 4‑bit rows, 0..15 */
#define DIFFS 8        /* 3‑bit diff masks, 0..7 */
#define STATE_SIZE (ROWS * DIFFS)

/* Allowed diff masks: 000,100,110,111 (non‑increasing in columns) */
static const bool allowed[DIFFS] = {
    true,   /* 000 */
    false,  /* 001 */
    false,  /* 010 */
    false,  /* 011 */
    true,   /* 100 */
    false,  /* 101 */
    true,   /* 110 */
    true    /* 111 */
};

/* Pre‑computed diff mask for every pair of rows.
   diff_mask[r1][r2] gives the 3‑bit mask for the 2×2 blocks between r1 and r2. */
static uint8_t diff_mask[ROWS][ROWS];
static bool diff_mask_computed = false;

/* Function that returns the number of valid (n+1)×4 0/1 arrays. */
seq_t a(idx_t n)
{
    /* Compute diff_mask on first call only. */
    if (!diff_mask_computed) {
        for (idx_t r1 = 0; r1 < ROWS; ++r1) {
            for (idx_t r2 = 0; r2 < ROWS; ++r2) {
                uint8_t mask = 0;
                for (idx_t j = 0; j < 3; ++j) {
                    unsigned a = (r1 >> j) & 1u;
                    unsigned b = (r1 >> (j + 1)) & 1u;
                    unsigned c = (r2 >> j) & 1u;
                    unsigned d = (r2 >> (j + 1)) & 1u;
                    unsigned ones = a + b + c + d;
                    unsigned bit = (ones == 2 || ones == 3) ? 1 : 0;
                    mask |= bit << j;
                }
                diff_mask[r1][r2] = mask;
            }
        }
        diff_mask_computed = true;
    }

    /* DP arrays: dp[state] = number of ways to reach this state. */
    seq_t dp[STATE_SIZE];
    seq_t next[STATE_SIZE];

    /* Initialise dp to zero. */
    for (idx_t i = 0; i < STATE_SIZE; ++i) dp[i] = 0;

    /* Initial transition: choose first two rows r0 and r1. */
    for (idx_t r0 = 0; r0 < ROWS; ++r0) {
        for (idx_t r1 = 0; r1 < ROWS; ++r1) {
            uint8_t d = diff_mask[r0][r1];
            if (!allowed[d]) continue;
            idx_t idx = (r1 << 3) | d; /* state = last row * 8 + diff mask */
            dp[idx] += (seq_t)1;
        }
    }

    /* If only one 2×2 subblock (n == 1), answer is sum of dp. */
    if (n == 1) {
        seq_t ans = 0;
        for (idx_t i = 0; i < STATE_SIZE; ++i) ans += dp[i];
        return ans;
    }

    /* Subsequent rows: step from 2 to n (i.e., add rows 3 … n+1). */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero next array. */
        for (idx_t i = 0; i < STATE_SIZE; ++i) next[i] = 0;

        for (idx_t state = 0; state < STATE_SIZE; ++state) {
            if (dp[state] == 0) continue;
            idx_t r_prev = state >> 3;   /* previous row (row i-1) */
            uint8_t d_prev = state & 7;   /* diff mask between row i-2 and i-1 */

            for (idx_t r_next = 0; r_next < ROWS; ++r_next) {
                uint8_t d_next = diff_mask[r_prev][r_next];
                if (!allowed[d_next]) continue;
                /* Vertical constraint: d_next must be a superset of d_prev. */
                if ((d_next | d_prev) != d_next) continue;
                idx_t new_state = (r_next << 3) | d_next;
                next[new_state] += dp[state];
            }
        }

        /* Copy next to dp for the next iteration. */
        for (idx_t i = 0; i < STATE_SIZE; ++i) dp[i] = next[i];
    }

    /* Sum all final states. */
    seq_t ans = 0;
    for (idx_t i = 0; i < STATE_SIZE; ++i) ans += dp[i];
    return ans;
}
