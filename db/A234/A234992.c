/*
 * A234992  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)x2 arrays over {0,…,6} with no equal
 *            adjacent entries and each 2x2 subblock having
 *            |(top0+bottom1)-(bottom0+top1)| = 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Externally linked function required by the build system */
seq_t a(idx_t n);

/* ----- Internal state handling ----- */
#define NUM_STATES 42

static uint8_t state_t[NUM_STATES];
static uint8_t state_b[NUM_STATES];
static int next_idx[NUM_STATES][NUM_STATES];
static unsigned char next_count[NUM_STATES];
static bool initialized = false;

static void init_states(void)
{
    if (initialized) return;
    int idx = 0;
    for (uint8_t t = 0; t < 7; ++t)
        for (uint8_t b = 0; b < 7; ++b)
            if (t != b) {
                state_t[idx] = t;
                state_b[idx] = b;
                ++idx;
            }
    for (int i = 0; i < NUM_STATES; ++i) next_count[i] = 0;
    for (int i = 0; i < NUM_STATES; ++i) {
        uint8_t t0 = state_t[i];
        uint8_t b0 = state_b[i];
        for (int j = 0; j < NUM_STATES; ++j) {
            uint8_t t1 = state_t[j];
            uint8_t b1 = state_b[j];
            if (t0 == t1 || b0 == b1) continue;           /* horizontal or vertical equality forbidden */
            int diff = (int)(t0 + b1) - (int)(b0 + t1);
            if (diff < 0) diff = -diff;
            if (diff == 2) {
                next_idx[i][next_count[i]++] = j;
            }
        }
    }
    initialized = true;
}

/* ----- Sequence computation using DP ----- */
seq_t a(idx_t n)
{
    init_states();
    idx_t columns = n + 1;          /* number of columns in the array */

    seq_t *dp_prev = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
    seq_t *dp_cur  = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
    if (!dp_prev || !dp_cur) abort();

    /* Initial column: any state is allowed (42 possibilities) */
    for (int i = 0; i < NUM_STATES; ++i) dp_prev[i] = 1;

    /* Perform L-1 transitions */
    for (idx_t step = 1; step <= columns - 1; ++step) {
        for (int i = 0; i < NUM_STATES; ++i) dp_cur[i] = 0;
        for (int i = 0; i < NUM_STATES; ++i) {
            seq_t val = dp_prev[i];
            if (val == 0) continue;
            unsigned char cnt = next_count[i];
            for (unsigned char k = 0; k < cnt; ++k) {
                int j = next_idx[i][k];
                dp_cur[j] = dp_cur[j] + val;
            }
        }
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur = tmp;
    }

    /* Sum over all ending states to get the total number of arrays */
    seq_t result = 0;
    for (int i = 0; i < NUM_STATES; ++i) result = result + dp_prev[i];

    free(dp_prev);
    free(dp_cur);
    return result;
}
