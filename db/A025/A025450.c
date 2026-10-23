/*
 * A025450  Number of partitions of n into 6 nonnegative cubes.
 * https://oeis.org/A025450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 6 nonnegative cubes * (0,
 *            1³, 2³, …). Equivalently, the number of partitions of n into at
 *            * most six positive cubes, because zeros can be added to reach
 *            six * parts without affecting the sum. The implementation uses
 *            an * unbounded‑coin DP counting combination...
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_cube_root.h"


seq_t a(idx_t n)
{
    /* Maximal cube root required */
    idx_t max_root = int_cube_root(n);

    /* dp[s][k] – number of ways to achieve sum s using exactly k positive
     * cubes processed so far.  k ranges 0..6, therefore a table of
     * (n+1) × 7 elements is sufficient. */
    size_t rows = 7;                 /* indices 0..6 */
    size_t cols = (size_t)n + 1;
    size_t total = rows * cols;
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp)
        return 0;    /* allocation failure */

    /* Base case: one way to reach sum 0 with 0 cubes */
    dp[0] = 1;

    /* Process each cube value v = i³, i = 1 … max_root */
    for (idx_t cube = 1; cube <= max_root; ++cube) {
        idx_t val = cube * cube * cube;   /* cube value <= n */

        /* In an unbounded coin DP, iterate sums upward so that each
         * value can be used repeatedly. */
        for (idx_t s = val; s <= n; ++s) {
            size_t s_idx = (size_t)s * rows;
            size_t prev_idx = (size_t)(s - val) * rows;
            for (int k = 1; k <= 6; ++k) {
                dp[s_idx + k] += dp[prev_idx + (k - 1)];
            }
        }
    }

    /* Sum contributions for all possible counts of non‑zero cubes.
     * Adding (6‑k) zeros to each such partition yields a unique
     * partition into exactly six cubes. */
    seq_t result = 0;
    size_t n_idx = (size_t)n * rows;
    for (int k = 0; k <= 6; ++k) {
        result += dp[n_idx + k];
    }

    free(dp);
    return result;
}
