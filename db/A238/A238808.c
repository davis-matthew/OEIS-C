/*
 * A238808  Number of n X 4 0..2 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of the sum of
 *          elements above it, modulo 3.
 * https://oeis.org/A238808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of n×4 arrays over {0,1,2} such that for every
 * cell (i,j) its value is not congruent to (1 + sum of the entries to its
 * left in the same row) or (1 + sum of the entries above it in the same
 * column) modulo 3.  Uses dynamic programming over the 4 column sums modulo
 * 3. */

#include <stddef.h>

#define STATE_COUNT 81

static idx_t col_sum[STATE_COUNT][4];
static unsigned char trans[STATE_COUNT][STATE_COUNT];

/* Recursively generate all possible rows for a given starting state `s`. */
static void dfs_from_state(idx_t s, int col, int lsum, int new_sums[4])
{
    if (col == 4) {
        /* Encode new column sums as a state id. */
        idx_t t = ((new_sums[0]*3 + new_sums[1])*3 + new_sums[2])*3 + new_sums[3];
        trans[s][t]++;                 /* trans values are tiny (≤16). */
        return;
    }
    int forbidden1 = (1 + lsum) % 3;
    int forbidden2 = (1 + col_sum[s][col]) % 3;
    for (int val = 0; val < 3; ++val) {
        if (val == forbidden1 || val == forbidden2) continue;
        new_sums[col] = (col_sum[s][col] + val) % 3;
        dfs_from_state(s, col + 1, (lsum + val) % 3, new_sums);
    }
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        /* Pre‑compute column‑sum tables for all 81 states. */
        for (idx_t s = 0; s < STATE_COUNT; ++s) {
            idx_t x = s;
            for (int j = 3; j >= 0; --j) {
                col_sum[s][j] = x % 3;
                x /= 3;
            }
        }

        /* Build the state transition table. */
        int new_sums[4];
        for (idx_t s = 0; s < STATE_COUNT; ++s) {
            dfs_from_state(s, 0, 0, new_sums);
        }
        initialized = 1;
    }

    /* Dynamic programming over rows. */
    seq_t dp[STATE_COUNT] = {0};
    seq_t ndp[STATE_COUNT];
    dp[0] = 1;                  /* initial column sums are all zero. */

    for (idx_t row = 0; row < n; ++row) {
        for (int i = 0; i < STATE_COUNT; ++i) ndp[i] = 0;
        for (idx_t s = 0; s < STATE_COUNT; ++s) {
            if (dp[s] == 0) continue;
            for (idx_t t = 0; t < STATE_COUNT; ++t) {
                unsigned char ways_u8 = trans[s][t];
                if (!ways_u8) continue;
                seq_t ways = (seq_t)ways_u8;
                ndp[t] += dp[s] * ways;
            }
        }
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = ndp[i];
    }

    /* Sum over all possible final states. */
    seq_t answer = 0;
    for (int i = 0; i < STATE_COUNT; ++i) answer += dp[i];
    return answer;
}
