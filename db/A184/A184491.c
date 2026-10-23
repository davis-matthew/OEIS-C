/*
 * A184491  Number of (n+1)X4 0..2 arrays with every 2X2 subblock sum greater
 *          than 4.
 * https://oeis.org/A184491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute a(n) by dynamic programming over row
   transitions for 0..2 4‑column arrays.  The value for the
   (n+1)×4 array count is the sum of the state counts after
   adding the (n+1)-th row.  No hard‑coded table is used. */

#include <stddef.h>
#include <stdint.h>

#define NUM_STATES 81   /* 3^4 possible rows */

/* Transition matrix and row decoding – built once. */
static uint8_t edges[NUM_STATES][NUM_STATES];
static uint8_t row_vals[NUM_STATES][4];
static int initialized = 0;

static void init_transitions(void)
{
    if (initialized)
        return;
    /* decode each row into its 4 digits (least significant first) */
    for (idx_t s = 0; s < NUM_STATES; ++s) {
        uint8_t t = (uint8_t)s;
        for (idx_t c = 0; c < 4; ++c) {
            row_vals[s][c] = t % 3;
            t /= 3;
        }
    }
    /* build transition matrix: edges[i][j] == 1 iff row i can precede row j */
    for (idx_t i = 0; i < NUM_STATES; ++i) {
        for (idx_t j = 0; j < NUM_STATES; ++j) {
            uint8_t ok = 1;
            for (idx_t col = 0; col < 3 && ok; ++col) {
                uint8_t a = row_vals[i][col];
                uint8_t b = row_vals[i][col + 1];
                uint8_t c = row_vals[j][col];
                uint8_t d = row_vals[j][col + 1];
                if ((a + b + c + d) <= 4)
                    ok = 0;
            }
            edges[i][j] = ok;
        }
    }
    initialized = 1;
}

/* Return a(n) for n >= 1.  For n == 0 the result is undefined
   but we return 0 for safety. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_transitions();

    /* DP state arrays.  Size 81 is small, so stack allocation is fine. */
    seq_t prev[NUM_STATES];
    seq_t next[NUM_STATES];

    /* start with one way for each possible first row */
    for (idx_t i = 0; i < NUM_STATES; ++i)
        prev[i] = (seq_t)1;

    for (idx_t step = 1; step <= n; ++step) {
        /* zero next row counts */
        for (idx_t i = 0; i < NUM_STATES; ++i)
            next[i] = (seq_t)0;

        /* transition from all previous rows to possible next rows */
        for (idx_t i = 0; i < NUM_STATES; ++i) {
            if (prev[i] == 0)
                continue;
            for (idx_t j = 0; j < NUM_STATES; ++j) {
                if (edges[i][j])
                    next[j] += prev[i];
            }
        }

        /* compute sum of all valid matrices of current height */
        seq_t total = (seq_t)0;
        for (idx_t i = 0; i < NUM_STATES; ++i)
            total += next[i];

        if (step == n)
            return total;

        /* copy next to prev for the next iteration */
        for (idx_t i = 0; i < NUM_STATES; ++i)
            prev[i] = next[i];
    }

    /* control should never reach here */
    return 0;
}
