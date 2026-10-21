/*
 * A235675  Number of (n+1)X(4+1) 0..1 arrays with the difference between each
 *          2X2 subblock maximum and minimum lexicographically nondecreasing
 *          columnwise and nonincreasing rowwise.
 * https://oeis.org/A235675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 binary arrays with the * difference
 *            between each 2×2 subblock maximum and minimum *
 *            lexicographically nondecreasing columnwise and nonincreasing *
 *            rowwise, computed by a DP over rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   row_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/row_diff.h"

static unsigned char diff_tbl[32][32];
static signed char diff_index[16];
static int initialized = 0;

/* Build the difference tables and pattern lookup once. */
static void build_tables(void)
{
    if (initialized) return;
    initialized = 1;

    /* All possible patterns that are lexicographically
     * nondecreasing from left to right. */
    const unsigned char valid_patterns[5] = {0, 1, 3, 7, 15};

    /* Map pattern value to its index 0..4; else -1. */
    for (int i = 0; i < 16; ++i) diff_index[i] = -1;
    for (int i = 0; i < 5; ++i) diff_index[valid_patterns[i]] = i;

    /* Precompute diff_tbl. */
    for (int r0 = 0; r0 < 32; ++r0)
        for (int r1 = 0; r1 < 32; ++r1)
            diff_tbl[r0][r1] = row_diff((unsigned char)r0, (unsigned char)r1);
}

/* Sequence term function. */
seq_t a(idx_t n)
{
    /* n >= 1 for the defined sequence. */
    if (n == 0) return (seq_t)0;

    build_tables();

    /* DP arrays: dp[p][r] = number of ways to reach row r
     * with the last 2×2 difference pattern having index p. */
    seq_t dp[5][32];
    seq_t next_dp[5][32];

    /* Initialize all counts to 0. */
    for (int p = 0; p < 5; ++p)
        for (int r = 0; r < 32; ++r)
            dp[p][r] = (seq_t)0;

    /* First transition: choose any pair of rows (r0 -> r1).
     * The pattern index of the difference is whatever it is. */
    for (int r0 = 0; r0 < 32; ++r0) {
        for (int r1 = 0; r1 < 32; ++r1) {
            unsigned char d = diff_tbl[r0][r1];
            int p = diff_index[d];
            if (p >= 0) {
                dp[p][r1] = dp[p][r1] + (seq_t)1;
            }
        }
    }

    /* If n == 1, we have already made one transition (two rows). */
    if (n > 1) {
        /* Perform additional (n-1) transitions. */
        for (idx_t step = 2; step <= n; ++step) {

            /* Zero next_dp. */
            for (int p = 0; p < 5; ++p)
                for (int r = 0; r < 32; ++r)
                    next_dp[p][r] = (seq_t)0;

            /* Transition from current state to next rows. */
            for (int p_cur = 0; p_cur < 5; ++p_cur) {
                for (int r_cur = 0; r_cur < 32; ++r_cur) {
                    seq_t cur = dp[p_cur][r_cur];
                    if (cur == (seq_t)0) continue;
                    for (int r_next = 0; r_next < 32; ++r_next) {
                        unsigned char d = diff_tbl[r_cur][r_next];
                        int p_next = diff_index[d];
                        if (p_next >= 0 && p_next <= p_cur) {
                            next_dp[p_next][r_next] = next_dp[p_next][r_next] + cur;
                        }
                    }
                }
            }

            /* Swap dp and next_dp for the next iteration. */
            for (int p = 0; p < 5; ++p)
                for (int r = 0; r < 32; ++r)
                    dp[p][r] = next_dp[p][r];
        }
    }

    /* Sum all counts: any pattern index and any final row are allowed. */
    seq_t result = (seq_t)0;
    for (int p = 0; p < 5; ++p)
        for (int r = 0; r < 32; ++r)
            result = result + dp[p][r];

    return result;
}
