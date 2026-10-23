/*
 * A026827  Number of partitions of n into distinct parts, the least being 6.
 * https://oeis.org/A026827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts with the
 *            smallest part equal to 6 (i.e., the part 6 must be present and
 *            all other parts are distinct and larger than 6)
 * Range:     u8 n <= 60, u16 n <= 124, u32 n <= 313, u64 n <= 925,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of A026827.  */
seq_t a(idx_t n)
{
    /* For n < 6 no partition can contain the obligatory part 6. */
    if (n < 6)
        return 0;

    /* Compute the amount that must be partitioned with distinct parts
       >= 7.  Part 6 is already used once. */
    idx_t m = n - 6;

    /* Allocate a DP table for sums 0..m.  */
    size_t sz = (size_t)(m + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)
        return 0;                /* allocation failure – return 0 as safe default */

    for (idx_t i = 0; i <= m; ++i)
        dp[i] = 0;
    dp[0] = 1;                    /* empty partition for sum 0 */

    /* Classical subset‑sum DP for distinct parts.  */
    for (idx_t part = 7; part <= m; ++part) {
        for (idx_t s = m; s >= part; --s) {
            dp[s] += dp[s - part];
        }
    }

    seq_t result = dp[m];
    free(dp);
    return result;
}
