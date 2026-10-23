/*
 * A036021  Number of partitions of n into parts not of form 4k+2, 16k, 16k+3
 *          or 16k-3.
 * https://oeis.org/A036021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not congruent to *
 *            0, 2, 3, 6, 10, 13, or 14 modulo 16
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a classic coin‑change dynamic programming
 * algorithm over the allowed part sizes. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array of seq_t values.  The array holds dp[i]
     * = number of partitions of i using only allowed parts.  */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* allocation failure – unlikely for small n */

    dp[0] = (seq_t)1;              /* empty partition */

    for (idx_t p = 1; p <= n; ++p) {
        /* Determine if part size p is allowed.  The forbidden
         * residues modulo 16 are 0, 2, 3, 6, 10, 13, and 14. */
        idx_t r = p & 15;          /* p % 16 using bitwise AND */
        if (r == 0 || r == 2 || r == 3 ||
            r == 6 || r == 10 || r == 13 || r == 14)
            continue;              /* skip forbidden part sizes */

        /* Unbounded coin change update: for each i ≥ p,
         * add the number of partitions of i-p to dp[i]. */
        for (idx_t i = p; i <= n; ++i)
            dp[i] = dp[i] + dp[i - p];
    }

    /* Copy the result and free the temporary array. */
    seq_t result = dp[n];
    free(dp);
    return result;
}
