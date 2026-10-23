/*
 * A195848  Expansion of 1 / f(-x^1, -x^5) in powers of x where f() is
 *          Ramanujan's two-variable theta function.
 * https://oeis.org/A195848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts congruent to 0, 1,
 *            or 5 (mod 6) using dynamic programming
 * Range:     u8 n <= 34, u16 n <= 88, u32 n <= 258, u64 n <= 838,
 *            u128 n <= 2925, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    if (dp == NULL || n > max_n) {
        seq_t *new_dp = alloc_seq_array((size_t)(n + 1));
        /* initialize */
        for (idx_t i = 0; i <= n; ++i) {
            new_dp[i] = 0;
        }
        new_dp[0] = 1;

        /* DP over allowed part sizes: 0,1,5 (mod 6). */
        for (idx_t c = 1; c <= n; ++c) {
            if (c % 6 == 0 || c % 6 == 1 || c % 6 == 5) {
                for (idx_t i = c; i <= n; ++i) {
                    new_dp[i] += new_dp[i - c];
                }
            }
        }

        if (dp != NULL) {
            free_seq_array(dp);
        }
        dp = new_dp;
        max_n = n;
    }
    return dp[n];
}
