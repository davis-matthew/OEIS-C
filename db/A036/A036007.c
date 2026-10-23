/*
 * A036007  Number of partitions of n into parts not of the form 25k, 25k+8 or
 *          25k-8. Also number of partitions with at most 7 parts of size 1
 *          and differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to 0,
 *            8, or 17 modulo 25, where a(1)=1, a(2)=2, ... (offset 1)
 * Range:     u8 n <= 16, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple dynamic programming
 * recurrence:
 *
 *   dp[0] = 1
 *   for each allowed part size s (1 <= s <= n)
 *       for i = s .. n
 *           dp[i] += dp[i - s];
 *
 * The table is memoised statically, so subsequent calls may reuse
 * previously computed values.  Memory is reallocated when a larger
 * `n` is requested, and the table is recomputed from scratch in
 * the new size.  This is sufficient for the required limit
 * (n up to ~10⁴) while keeping the code short and portable.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; a(1) is the first term. */
    if (n == 0)
        return (seq_t)0;

    /* Static memoization storage. */
    static seq_t *dp = NULL;
    static idx_t max_n = 0;   /* highest n that has been computed */

    /* If a larger n is requested, recompute the table in the new range. */
    if (n > max_n) {
        size_t new_size = (size_t)n + 1;        /* include 0..n */

        /* Allocate/resize the dp array.  Casting the result of realloc
         * is required both for C and C++.  The seq_t type may be a
         * POD or a simple class; realloc works because we only store
         * and read values. */
        dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!dp)
            return (seq_t)0;  /* allocation failure – return 0 */

        /* Initialize all entries to zero. */
        for (idx_t i = 0; i <= n; ++i)
            dp[i] = (seq_t)0;

        /* Base case for partition counting. */
        dp[0] = (seq_t)1;

        /* Run the DP for the new maximum size. */
        for (idx_t part = 1; part <= n; ++part) {
            /* Skip forbidden part sizes (multiples of 25, 25k+8,
               and 25k-8 which is congruent to 17). */
            idx_t r = part % 25;
            if (r == 0 || r == 8 || r == 17)
                continue;

            for (idx_t sum = part; sum <= n; ++sum) {
                dp[sum] += dp[sum - part];
            }
        }

        /* Remember the new maximum for future calls. */
        max_n = n;
    }

    return dp[n];
}
