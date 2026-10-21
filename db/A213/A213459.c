/*
 * A213459  Number of 0..3 arrays of length n+5 with sum less than 9 in any
 *          length 6 subsequence (=less than 50% duty cycle).
 * https://oeis.org/A213459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..3 arrays of length n+5 such that every *
 *            contiguous subsequence of length 6 has sum <= 8. The value is *
 *            computed by a dynamic programming that keeps the last five
 *            digits * of the array
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 32, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 1024
#define STATE_MASK  1023

static int sum_state[STATE_COUNT];
static int next_state[STATE_COUNT][4];
static bool  allowed_state[STATE_COUNT][4];
static bool  tables_initialised = false;

/* DP function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the offset is 1; 0 is undefined */

    /* initialise transition tables once */
    if (!tables_initialised) {
        for (int s = 0; s < STATE_COUNT; ++s) {
            int tmp = s;
            int sum = 0;
            for (int i = 0; i < 5; ++i) {
                sum += tmp & 3;
                tmp >>= 2;
            }
            sum_state[s] = sum;
            for (int v = 0; v < 4; ++v) {
                next_state[s][v] = ((v << 8) | (s >> 2)) & STATE_MASK;
                allowed_state[s][v] = (sum + v <= 8);
            }
        }
        tables_initialised = true;
    }

    const idx_t length = n + 5;      /* length of the array to build */

    seq_t dp[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];

    /* initialise dp arrays */
    for (int i = 0; i < STATE_COUNT; ++i) {
        dp[i] = 0;
        dp_next[i] = 0;
    }
    dp[0] = 1;  /* start with all five positions empty (value 0) */

    for (idx_t pos = 0; pos < length; ++pos) {
        /* zero next DP state */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;

            for (int v = 0; v < 4; ++v) {
                if (pos < 5 || allowed_state[s][v]) {
                    int ns = next_state[s][v];
                    dp_next[ns] += cur;
                }
            }
        }

        /* swap dp and dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = dp_next[i];
    }

    /* sum all states to get the total number of valid arrays */
    seq_t ans = 0;
    for (int i = 0; i < STATE_COUNT; ++i) ans += dp[i];

    return ans;
}
