/*
 * A221594  Number of 0..6 arrays of length n with each element differing from
 *          at least one neighbor by 1 or less.
 * https://oeis.org/A221594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..6 arrays of length n in which each element
 *            differs from at least one neighbor by at most 1. Computed by
 *            dynamic programming with states (last value, unsatisfied flag).
 *            Unsatisfied flag = 1 means the last element has not yet seen a
 *            neighbor with difference <= 1; it must be sati...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined for n<1, but return 0 */

    /* dp[v][f] for current length, next[v][f] for next length */
    seq_t dp[7][2] = {0};
    seq_t next[7][2] = {0};

    /* initialize for length 1: all values unsatisfied (no left neighbor) */
    for (int v = 0; v < 7; ++v)
        dp[v][1] = (seq_t)1;

    /* iterate for lengths 1 to n-1 */
    for (idx_t i = 1; i < n; ++i) {
        /* zero next array */
        for (int v = 0; v < 7; ++v) {
            next[v][0] = (seq_t)0;
            next[v][1] = (seq_t)0;
        }

        for (int v = 0; v < 7; ++v) {
            for (int f = 0; f < 2; ++f) {
                seq_t cur = dp[v][f];
                if (cur == (seq_t)0) continue;
                for (int w = 0; w < 7; ++w) {
                    int diff = v >= w ? v - w : w - v;
                    if (f == 1 && diff > 1) continue;        /* must satisfy old */
                    int new_f = (diff <= 1) ? 0 : 1;          /* new's status */
                    next[w][new_f] += cur;
                }
            }
        }

        /* move next to dp */
        for (int v = 0; v < 7; ++v) {
            dp[v][0] = next[v][0];
            dp[v][1] = next[v][1];
        }
    }

    /* sum over last element satisfied flag = 0 */
    seq_t result = (seq_t)0;
    for (int v = 0; v < 7; ++v)
        result += dp[v][0];

    return result;
}
