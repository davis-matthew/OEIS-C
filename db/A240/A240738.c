/*
 * A240738  Number of compositions of n having exactly three fixed points.
 * https://oeis.org/A240738
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n having exactly three fixed
 *            points The recurrence counts compositions of length i by
 *            extending compositions of length i-1 with part i or another
 *            part. The DP runs in O(n²) time and uses four counters for the
 *            number of fixed points (0..3)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t N = n;
    if (N == 0) return 0;

    /* Allocate two DP arrays: dp_prev and dp_new.
       Each stores values for sums 0..N and fixed-point counts 0..3. */
    const size_t DIM = (size_t)(N + 1) * 4;
    seq_t *dp_prev = (seq_t *)calloc(DIM, sizeof(seq_t));
    seq_t *dp_new  = (seq_t *)calloc(DIM, sizeof(seq_t));
    if (!dp_prev || !dp_new) {
        free(dp_prev);
        free(dp_new);
        return 0;  /* allocation failure, return 0 as safe fallback */
    }

    /* Initial state: sum 0, 0 fixed points -> one way (empty composition). */
    dp_prev[0 * 4 + 0] = 1;

    seq_t result = 0;

    /* Iterate over positions i = 1 .. N.  After processing i we have
       all compositions of length i.  We collect contributions for
       sum == N and exactly 3 fixed points. */
    for (idx_t i = 1; i <= N; ++i) {
        /* Prefix sums of dp_prev over the first dimension (sum). */
        seq_t sum_m[4] = {0, 0, 0, 0};

        for (idx_t s = 0; s <= N; ++s) {
            /* Compute dp_new[s][m] for all m = 0..3. */
            for (int m = 0; m < 4; ++m) {
                seq_t val = sum_m[m];  /* sum of dp_prev[0..s-1][m] */
                if (s >= i) {
                    /* Part a_i = i contributes a fixed point. */
                    val -= dp_prev[(s - i) * 4 + m];
                    if (m >= 1)
                        val += dp_prev[(s - i) * 4 + (m - 1)];
                }
                dp_new[s * 4 + m] = val;
            }

            /* Update the running prefix sums for the next s. */
            for (int m = 0; m < 4; ++m) {
                sum_m[m] += dp_prev[s * 4 + m];
            }
        }

        /* Accumulate the answer for this length i. */
        result += dp_new[N * 4 + 3];

        /* Prepare for the next iteration. Swap pointers. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_new;
        dp_new = tmp;
        /* dp_new will be overwritten in the next iteration, no need to clear. */
    }

    free(dp_prev);
    free(dp_new);
    return result;
}
