/*
 * A126536  Number of base 15 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126536: Number of base‑15 n‑digit strings with adjacent digits\
 * differing by at most five.  Computed by dynamic programming on an adjacency\
 * matrix of size 15, using prefix sums for each step. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle offset. a(0) = 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* DP arrays for counts at current length. */
    seq_t dp[15];
    seq_t next[15];
    /* All one‑digit strings. */
    for (int j = 0; j < 15; ++j) {
        dp[j] = (seq_t)1;
    }

    /* Iterate lengths from 2 up to n. */
    for (idx_t len = 2; len <= n; ++len) {
        /* Prefix sums of dp for quick window sums. */
        seq_t prefix[16];
        prefix[0] = (seq_t)0;
        for (int i = 0; i < 15; ++i) {
            prefix[i + 1] = prefix[i] + dp[i];
        }

        /* compute next[j] = sum_{i=|j-i|<=5} dp[i] */
        for (int j = 0; j < 15; ++j) {
            int left  = j - 5;
            if (left < 0) left = 0;
            int right = j + 5;
            if (right > 14) right = 14;
            next[j] = prefix[right + 1] - prefix[left];
        }

        /* copy next into dp for next iteration */
        for (int j = 0; j < 15; ++j) {
            dp[j] = next[j];
        }
    }

    /* Sum over last dp to get a(n). */
    seq_t total = (seq_t)0;
    for (int j = 0; j < 15; ++j) {
        total += dp[j];
    }
    return total;
}
