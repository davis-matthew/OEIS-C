/*
 * A008650  Molien series of 4 X 4 upper triangular matrices over GF( 3 ).
 * https://oeis.org/A008650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n using parts 1, 3, 9, and 27
 * Range:     u8 n <= 83, u16 n <= 638, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming with an incremental
   cache that is rebuilt only when a larger index is requested. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values.  The cache is
       rebuilt from scratch only when the requested index n exceeds
       the current cache size, guaranteeing correctness without
       double‑counting. */
    static seq_t *dp = NULL;      /* dynamic array of partition counts */
    static idx_t dp_len = 0;      /* number of elements in dp (0 means empty) */

    /* If the cache is empty, create it and fill with zeros. */
    if (dp_len == 0) {
        dp_len = 1;               /* at least index 0 */
        dp = (seq_t *)malloc(dp_len * sizeof(seq_t));
        if (!dp) return 0;        /* memory failure */
        dp[0] = 1;                /* base case: one way to partition 0 */
    }

    /* Extend the cache if necessary.  When extending we rebuild the
       entire table up to the new size because the old entries may
       already depend on larger parts.  This keeps the recurrence
       simple and correct. */
    if (n + 1 > dp_len) {
        idx_t old_len = dp_len;
        dp_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, dp_len * sizeof(seq_t));
        if (!tmp) {
            free(dp);           /* cleanup on failure */
            return 0;
        }
        dp = tmp;

        /* Reinitialise the array to zero (except for index 0).  The
           realloc may not zero the new memory, so we must do it
           explicitly. */
        for (idx_t i = 1; i < dp_len; ++i) dp[i] = 0;
        dp[0] = 1;

        /* Parts that are allowed in the partitions. */
        static const idx_t parts[] = {1, 3, 9, 27};
        const unsigned num_parts = 4;

        /* Dynamic programming: for each part, add its contribution to all
           larger indices.  This guarantees that each partition is counted
           exactly once, regardless of the order of parts. */
        for (unsigned pi = 0; pi < num_parts; ++pi) {
            idx_t p = parts[pi];
            for (idx_t i = p; i < dp_len; ++i) {
                dp[i] += dp[i - p];
            }
        }
    }

    return dp[n];
}
