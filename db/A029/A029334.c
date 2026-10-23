/*
 * A029334  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^7)*(1-x^10)).
 * https://oeis.org/A029334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A029334: number of partitions of n into parts 4, 5, 7 and 10.
   This uses a classic unbounded‑knapsack DP that builds a table of
   partition counts up to the largest n requested so far.  The table
   is recomputed whenever a larger n is needed.  The implementation
   works for any seq_t type (unsigned integer or arbitrary‑precision
   class) because it only uses +, =, and writes to array elements. */

#include <stdlib.h>   /* malloc, free */

/* The function defined by the framework */
seq_t a(idx_t n)
{
    /* static cache of partition counts up to the greatest n seen so far */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* number of entries in dp (dp_size == max_n+1) */

    /* initialisation on first call */
    if (dp_size == 0) {
        dp_size = 1;                    /* we at least need space for n = 0 */
        dp = (seq_t*)malloc(dp_size * sizeof(seq_t));
        if (!dp) return (seq_t)0;        /* unlikely: abort on OOM */
        dp[0] = 1;                       /* only one way to obtain 0 */
    }

    /* if  requesting a larger n than we have computed, rebuild the table   */
    if (n >= dp_size) {
        idx_t new_size = n + 1;          /* array must hold indices 0 … n   */
        seq_t *new_dp = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_dp) return (seq_t)0;    /* abort on OOM                     */

        /* zero initialise and set base case */
        for (idx_t i = 0; i < new_size; ++i) new_dp[i] = (seq_t)0;
        new_dp[0] = 1;

        /* The allowed part sizes */
        static const idx_t parts[] = {4, 5, 7, 10};
        const size_t parts_len = sizeof(parts) / sizeof(parts[0]);

        /* classic DP: for each part, update the table */
        for (size_t p_idx = 0; p_idx < parts_len; ++p_idx) {
            idx_t p = parts[p_idx];
            for (idx_t i = p; i <= n; ++i) {
                new_dp[i] = new_dp[i] + new_dp[i - p];
            }
        }

        /* free the old table and point to the new one */
        free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    /* return the cached value for the requested n */
    return dp[n];
}
