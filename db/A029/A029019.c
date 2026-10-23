/*
 * A029019  Expansion of 1/((1-x)*(1-x^2)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A029019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the number of partitions of n into the parts
   1, 2, 6, and 11 by dynamic programming.  The values are stored in a
   static array that is expanded as needed.  It works for any integer
   type seq_t supported by the build, including arbitrary‑precision
   integer classes.  The implementation uses only basic integer
   arithmetic and loops; no external helpers are required. */
#include <stdlib.h>

#ifdef __cplusplus
#  include <cstddef>
#endif

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static size_t dp_size = 0;   /* number of elements allocated (n+1) */

    /* Expand the table if necessary */
    if ((size_t)n + 1 > dp_size) {
        seq_t *new_dp;
#ifdef __cplusplus
        new_dp = new seq_t[(size_t)n + 1];
#else
        new_dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
#endif
        if (!new_dp) return 0;  /* allocation failure – return 0 */

        /* free old table */
        if (dp) {
#ifdef __cplusplus
            delete [] dp;
#else
            free(dp);
#endif
        }
        dp = new_dp;
        dp_size = (size_t)n + 1;

        /* initialise the table */
        for (size_t i = 0; i < dp_size; ++i) dp[i] = 0;
        dp[0] = 1;

        static const idx_t parts[4] = {1, 2, 6, 11};

        for (int p = 0; p < 4; ++p) {
            idx_t coin = parts[p];
            for (idx_t i = coin; i < (idx_t)dp_size; ++i) {
                dp[i] += dp[i - coin];
            }
        }
    }

    return dp[n];
}
