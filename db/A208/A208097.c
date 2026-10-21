/*
 * A208097  Number of 2 X n nonnegative integer arrays with new values 0
 *          upwards introduced in row major order and no element equal to any
 *          horizontal, diagonal or antidiagonal neighbor (colorings ignoring
 *          permutations of colors).
 * https://oeis.org/A208097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed via dynamic programming over 2×n arrays that
 *            are colored in row‑major order with the rule that a cell cannot
 *            equal any of its horizontal or diagonal neighbors. The algorithm
 *            tracks the number of distinct colors used and whether the last
 *            column contains one or two distinct colors
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* maximum possible number of distinct colors */
    idx_t maxK = 2 * n + 2;                 /* safety margin */

    /* dp0[k] – last column has one distinct color (f = 1)
       dp1[k] – last column has two distinct colors (f = 2) */
    seq_t *dp0 = (seq_t *)calloc((size_t)(maxK + 1), sizeof(seq_t));
    seq_t *dp1 = (seq_t *)calloc((size_t)(maxK + 1), sizeof(seq_t));

    /* initial column (j = 1) */
    dp0[1] = (seq_t)1;      /* both cells are the same color 0 */
    dp1[2] = (seq_t)1;      /* top 0, bottom 1 – two distinct colors */

    for (idx_t col = 2; col <= n; ++col) {
        /* next arrays for column 'col' */
        seq_t *next0 = (seq_t *)calloc((size_t)(maxK + 1), sizeof(seq_t));
        seq_t *next1 = (seq_t *)calloc((size_t)(maxK + 1), sizeof(seq_t));

        /* maximum k that can appear after previous column */
        idx_t maxPrevK = 2 * (col - 1) + 1;
        for (idx_t k = 1; k <= maxPrevK; ++k) {
            seq_t cnt = dp0[k];
            if (cnt) {
                idx_t f0 = 1;
                idx_t E0 = k >= f0 ? k - f0 : 0;   /* allowed existing colors */

                /* Top cell uses an existing color */
                if (E0 >= 1) {
                    /* Bottom cell same existing color */
                    next0[k] += cnt * (seq_t)E0;
                    /* Bottom cell different existing color */
                    next1[k] += cnt * (seq_t)(E0 * (E0 - 1));
                    /* Bottom cell introduces a new color */
                    next1[k + 1] += cnt * (seq_t)E0;
                }

                /* Top cell uses a new color */
                /* After top new, k1 = k + 1, allowed existing colors for bottom = k1 - f0 = E0 + 1 */
                /* Bottom cell picks the new color (same color pair) */
                next0[k + 1] += cnt;
                /* Bottom cell picks one of the other existing colors (distinct pair) */
                if (E0 >= 1) {
                    next1[k + 1] += cnt * (seq_t)E0;
                }
                /* Bottom cell introduces a new color (distinct pair) */
                next1[k + 2] += cnt;
            }

            cnt = dp1[k];
            if (cnt) {
                idx_t f0 = 2;
                if (k < 2) continue;   /* impossible state */
                idx_t E0 = k >= f0 ? k - f0 : 0;

                /* Top cell uses an existing color */
                if (E0 >= 1) {
                    next0[k] += cnt * (seq_t)E0;
                    next1[k] += cnt * (seq_t)(E0 * (E0 - 1));
                    next1[k + 1] += cnt * (seq_t)E0;
                }

                /* Top cell uses a new color */
                /* After top new, k1 = k + 1, allowed existing colors for bottom = k1 - f0 = k - 1 = E0 + 1 */
                next0[k + 1] += cnt;
                if (E0 >= 1) {
                    next1[k + 1] += cnt * (seq_t)E0;
                }
                next1[k + 2] += cnt;
            }
        }

        free(dp0);
        free(dp1);
        dp0 = next0;
        dp1 = next1;
    }

    /* Sum all states after processing n columns */
    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= maxK; ++k) {
        result += dp0[k];
        result += dp1[k];
    }

    free(dp0);
    free(dp1);
    return result;
}
