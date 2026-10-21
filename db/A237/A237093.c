/*
 * A237093  Number of (n+1)X(2+1) 0..2 arrays with the maximum plus the lower
 *          median minus the upper median of every 2X2 subblock equal.
 * https://oeis.org/A237093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2} such that every 2×2
 *            subblock satisfies max + lower_median – upper_median = constant
 *            K. Computed by dynamic programming on row states
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   compute_block_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_block_value.h"

/* Constants for the DP */
#define ROWS 27          /* 3³ possible rows */
#define MAX_CONST 3      /* possible values of the constant K */

/* Global tables (initialised once) */
static bool init_done = false;
static uint8_t rows[ROWS][3];          /* all possible rows */
static int8_t allowed[ROWS][ROWS];     /* -1 means forbidden, otherwise 0..2 */

/* One‑time initialisation of the tables */
static void init_globals(void)
{
    if (init_done)
        return;
    init_done = true;

    /* Enumerate all 3³ rows (base‑3 representation). */
    for (int code = 0; code < ROWS; ++code) {
        int temp = code;
        for (int f = 0; f < 3; ++f) {
            rows[code][f] = (uint8_t)(temp % 3);
            temp /= 3;
        }
    }

    /* Pre‑compute allowed transitions and the corresponding constant K. */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int left  = compute_block_value(rows[i][0], rows[i][1],
                                            rows[j][0], rows[j][1]);
            int right = compute_block_value(rows[i][1], rows[i][2],
                                            rows[j][1], rows[j][2]);
            allowed[i][j] = (left == right) ? (int8_t)left : -1;
        }
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    init_globals();

    seq_t total = 0;

    /* Iterate over each possible constant K (0,1,2). */
    for (int K = 0; K < MAX_CONST; ++K) {
        /* DP vectors: cur[i] = number of ways to reach row i after current step. */
        seq_t cur[ROWS], next[ROWS];

        /* Start with any first row (n+1 rows ⇒ n transitions). */
        for (int i = 0; i < ROWS; ++i)
            cur[i] = (seq_t)1;

        /* Perform n transitions (appending n rows after the first). */
        for (idx_t step = 0; step < n; ++step) {
            for (int i = 0; i < ROWS; ++i)
                next[i] = (seq_t)0;

            for (int from = 0; from < ROWS; ++from) {
                if (cur[from] == (seq_t)0)
                    continue;
                for (int to = 0; to < ROWS; ++to) {
                    if (allowed[from][to] == K)
                        next[to] += cur[from];
                }
            }

            /* Prepare for next step. */
            for (int i = 0; i < ROWS; ++i)
                cur[i] = next[i];
        }

        /* Sum all ending rows for this K. */
        seq_t sumK = 0;
        for (int i = 0; i < ROWS; ++i)
            sumK += cur[i];

        total += sumK;
    }

    return total;
}
