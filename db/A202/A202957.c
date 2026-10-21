/*
 * A202957  Number of arrays of n+2 integers in -3..3 with sum zero and
 *          adjacent elements differing in absolute value.
 * https://oeis.org/A202957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays of n+2 integers in -3..3 with sum zero
 *            and adjacent elements differing in absolute value
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* length of the arrays */
    idx_t L = n + 2;

    /* maximum possible absolute sum: 3 * L                     */
    /* number of integer sums from -3*L to 3*L inclusive:      */
    /*   size = 6*L + 1                                         */
    size_t offset = 3 * L;
    size_t range  = 6 * L + 1;            /* 0 … 6*L           */

    /* allocate DP tables for the four possible absolute values
       (0,1,2,3).  Two copies are used to alternate between
       current and next states.                                 */
    seq_t *dp[4], *next[4];
    for (int a = 0; a < 4; ++a) {
        dp[a]   = (seq_t *)calloc(range, sizeof(seq_t));
        next[a] = (seq_t *)calloc(range, sizeof(seq_t));
    }

    /* ----- initialisation for length 1 --------------------------------- */
    /* choose any single element v in [-3,3].                           */
    for (int v = -3; v <= 3; ++v) {
        int abs = v >= 0 ? v : -v;                    /* |v|        */
        size_t idx = offset + (size_t)v;            /* index of the sum v            */
        switch (abs) {
        case 0: dp[0][idx] += 1; break;
        case 1: dp[1][idx] += 1; break;
        case 2: dp[2][idx] += 1; break;
        case 3: dp[3][idx] += 1; break;
        }
    }

    /* ----- DP for lengths 2 … L -------------------------------------- */
    for (idx_t len = 2; len <= L; ++len) {
        /* ranges of sums that are reachable at the previous length   */
        size_t cur_min = offset - 3 * (len - 1);
        size_t cur_max = offset + 3 * (len - 1);

        /* ranges that will be used by the next length (len)         */
        size_t new_min = offset - 3 * len;
        size_t new_max = offset + 3 * len;
        size_t new_len = new_max - new_min + 1;

        /* zero only the part of the new buffer that will be written */
        for (int a = 0; a < 4; ++a) {
            memset(next[a] + new_min, 0, new_len * sizeof(seq_t));
        }

        /* iterate over all currently reachable sums */
        for (size_t idx = cur_min; idx <= cur_max; ++idx) {
            seq_t tot = dp[0][idx] + dp[1][idx] + dp[2][idx] + dp[3][idx];
            seq_t self0 = dp[0][idx];
            seq_t self1 = dp[1][idx];
            seq_t self2 = dp[2][idx];
            seq_t self3 = dp[3][idx];

            /* abs value 0: only value 0 */
            next[0][idx] += tot - self0;

            /* abs value 1: values +1, -1 */
            seq_t add1 = tot - self1;
            next[1][idx + 1] += add1;
            next[1][idx - 1] += add1;

            /* abs value 2: values +2, -2 */
            seq_t add2 = tot - self2;
            next[2][idx + 2] += add2;
            next[2][idx - 2] += add2;

            /* abs value 3: values +3, -3 */
            seq_t add3 = tot - self3;
            next[3][idx + 3] += add3;
            next[3][idx - 3] += add3;
        }

        /* swap dp ↔ next for next iteration */
        for (int a = 0; a < 4; ++a) {
            seq_t *tmp = dp[a];
            dp[a] = next[a];
            next[a] = tmp;
        }
    }

    /* ----- result ----------------------------------------------------- */
    size_t zero_idx = offset;      /* sum = 0 */
    seq_t result = dp[0][zero_idx] + dp[1][zero_idx] +
                   dp[2][zero_idx] + dp[3][zero_idx];

    /* free temporary storage */
    for (int a = 0; a < 4; ++a) {
        free(dp[a]);
        free(next[a]);
    }

    return result;
}
