/*
 * A211697  Number of nonnegative integer arrays of length n+2*5-2 with new
 *          values introduced in order 0 upwards and every value appearing
 *          only in runs of at least 5.
 * https://oeis.org/A211697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 51, u64 n <= 92, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A211697: Number of nonnegative integer arrays of length n+8
 * with new values introduced in increasing order starting at 0,
 * and every value appearing only in runs of length at least 5.
 * 
 * The algorithm uses dynamic programming on
 *   dp[k][len]  – number of partial arrays that, after a
 *                 certain number of positions, have used exactly k
 *                 distinct values and whose current run has length
 *                 len, where len = 1..5 (len == 5 denotes a run
 *                 of length >=5).  The first element is always 0,
 *                 so the initial state is (k = 1, len = 1).
 *   
 * Transition rules (for next position):
 *   - If len < 5: the run must continue, new_len = len + 1.
 *   - If len == 5:
 *       * continue the current run (len stays 5),
 *       * start a new value (k + 1, len = 1) – allowed only
 *         if k + 1 <= k_max,
 *       * switch to any of the (k-1) previous values
 *         (k stays the same, len = 1).
 *
 * The maximum number of distinct values that can appear in an array
 * of length N is floor((N+4)/5).  For an array of length N = n+8
 * this value is k_max = (N + 4) / 5.
 *
 * Complexity:  O(N * k_max) = O(n^2 / 5), well below the limit
 * for n <= 10000.  Memory usage is O(k_max).
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* length of the array to be counted */
    idx_t N = n + 8;          /* L = n + 2*5 - 2 */

    /* maximum distinct values usable in an array of length N */
    idx_t k_max = (N + 4) / 5;   /* floor((N+4)/5) */

    /* number of entries in a flat dp array: (k_max+2) rows
     * each with 6 columns (indices 0..5, we use 1..5)   */
    size_t elems = (size_t)(k_max + 2) * 6;

    seq_t *dp_cur  = (seq_t *)calloc(elems, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(elems, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        /* allocation failed – terminate with zero result for safety */
        free(dp_cur);
        free(dp_next);
        return 0;
    }

    /* start with a single element 0, run length 1, distinct count 1 */
    dp_cur[1 * 6 + 1] = 1;

    /* iteratively add positions 2 .. N   (total N-1 steps) */
    for (idx_t pos = 1; pos < N; ++pos) {
        /* maximum number of distinct values that could have been
           used after 'pos' positions (integer division) */
        idx_t max_k = (pos + 4) / 5;
        if (max_k > k_max) max_k = k_max;

        /* clear next DP row */
        memset(dp_next, 0, elems * sizeof(seq_t));

        for (idx_t k = 1; k <= max_k; ++k) {
            for (int len = 1; len <= 5; ++len) {
                seq_t val = dp_cur[k * 6 + len];
                if (!val) continue;

                if (len < 5) {                     /* run must continue   */
                    int new_len = len + 1;
                    dp_next[k * 6 + new_len] += val;
                } else {                           /* len == 5             */
                    /* keep the current run running          */
                    dp_next[k * 6 + 5] += val;

                    /* start a new value (k+1) if allowed      */
                    if (k + 1 <= k_max)
                        dp_next[(k + 1) * 6 + 1] += val;

                    /* switch to any earlier value (k-1)       */
                    if (k > 1)
                        dp_next[k * 6 + 1] += val * (seq_t)(k - 1);
                }
            }
        }

        /* swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* accumulate results with a completed last run (len == 5) */
    seq_t result = 0;
    for (idx_t k = 1; k <= k_max; ++k)
        result += dp_cur[k * 6 + 5];

    free(dp_cur);
    free(dp_next);
    return result;
}
