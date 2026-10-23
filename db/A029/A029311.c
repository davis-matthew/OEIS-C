/*
 * A029311  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3, 7, 9, and 11
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values up to the largest n seen so far */
    static seq_t *dp       = NULL;
    static idx_t  capacity = 0;
    static idx_t  computed_max = 0;

    /* recompute DP from scratch whenever we need a larger n */
    if (dp == NULL || n > computed_max) {
        /* ensure the array is large enough for indices 0..n */
        idx_t new_capacity = n + 1;
        if (new_capacity > capacity) {
            seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * new_capacity);
            if (tmp == NULL) { /* allocation error – return 0 as safe default */
                return 0;
            }
            dp = tmp;
            capacity = new_capacity;
        }

        /* initialise all entries to 0 */
        for (idx_t i = 0; i <= n; ++i) {
            dp[i] = 0;
        }
        dp[0] = 1;  /* base case: one way to sum to 0 */

        /* parts to be used in the partitions */
        static const idx_t parts[4] = {3, 7, 9, 11};

        /* Classic coin–change DP: outer loop over parts, inner over sums */
        for (int p = 0; p < 4; ++p) {
            idx_t part = parts[p];
            for (idx_t i = part; i <= n; ++i) {
                dp[i] += dp[i - part];
            }
        }

        computed_max = n;
    }

    return dp[n];
}
