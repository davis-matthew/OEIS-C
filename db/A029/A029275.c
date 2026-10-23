/*
 * A029275  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^6)*(1-x^10)).
 * https://oeis.org/A029275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 695, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029275: Number of partitions of n into parts 3, 5, 6 and 10. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dp[i] holds the number of partitions of i.  The array is lazily
       reallocated to the necessary size when a larger n is requested. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* current size of dp (number of entries) */

    if (n >= dp_size) {
        /* Need to grow the table.  Allocate a new array large enough for
           indices 0 .. n.  Existing values are irrelevant because we will
           recompute the whole table. */
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!new_dp) {
            /* Out of memory – return 0 (unlikely for the ranges used). */
            return (seq_t)0;
        }

        /* Initialise entries to zero.  The first entry is 1 (empty partition). */
        for (idx_t i = 0; i < new_size; ++i) new_dp[i] = 0;
        new_dp[0] = 1;

        /* Parts that are allowed.  Add each part one after the other to
           accumulate the number of partitions. */
        const idx_t parts[] = {3, 5, 6, 10};
        const size_t parts_count = 4;

        for (size_t pi = 0; pi < parts_count; ++pi) {
            idx_t p = parts[pi];
            for (idx_t i = p; i <= n; ++i) {
                new_dp[i] += new_dp[i - p];
            }
        }

        /* Replace the old table (if any). */
        if (dp) free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}
