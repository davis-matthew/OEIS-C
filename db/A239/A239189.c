/*
 * A239189  Number of nX3 0..4 arrays with no element equal to the sum of
 *          elements to its left or the sum of elements above it, modulo 5.
 * https://oeis.org/A239189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over the states of
 *            column sums modulo 5 for an n×3 array. The transition matrix is
 *            built once, then each row is processed by multiplying the
 *            current state vector with this matrix
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function is fully type‑safe for all supported
    seq_t and idx_t variant builds.  No external files
    are read or written. */

#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 125   /* 5^3 possible column-sum states */

static uint16_t trans[STATE_COUNT][STATE_COUNT];
static bool trans_initialized = false;

/* Build the transition matrix once */
static void init_trans(void)
{
    if (trans_initialized) return;
    trans_initialized = true;

    for (int s = 0; s < STATE_COUNT; ++s) {
        int a = s / 25;             /* column‑1 sum above, modulo 5 */
        int b = (s / 5) % 5;        /* column‑2 sum above */
        int c = s % 5;              /* column‑3 sum above */

        for (int x1 = 0; x1 <= 4; ++x1) {
            if (x1 == 0 || x1 == a) continue;          /* left or above */
            for (int x2 = 0; x2 <= 4; ++x2) {
                if (x2 == x1 || x2 == b) continue;     /* left or above */
                int left3 = (x1 + x2) % 5;
                for (int x3 = 0; x3 <= 4; ++x3) {
                    if (x3 == left3 || x3 == c) continue;
                    int na = (a + x1) % 5;
                    int nb = (b + x2) % 5;
                    int nc = (c + x3) % 5;
                    int t = na * 25 + nb * 5 + nc;
                    trans[s][t] += 1;   /* Count one more row that
                                            leads from s to t */
                }
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range; offset starts at 1 */

    init_trans();                    /* Ensure transitions are ready */

    seq_t dp[STATE_COUNT]      = {0};
    seq_t dp_next[STATE_COUNT] = {0};

    dp[0] = 1;                     /* Start with all column sums 0 */

    for (idx_t row = 0; row < n; ++row) {
        /* Zero next vector */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* Transition */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            for (int t = 0; t < STATE_COUNT; ++t) {
                uint16_t ways = trans[s][t];
                if (ways == 0) continue;
                dp_next[t] += cur * (seq_t)ways;
            }
        }

        /* Prepare for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

    /* Sum all dp entries for final count */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += dp[i];
    return total;
}
