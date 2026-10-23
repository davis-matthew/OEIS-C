/*
 * A008640  Number of partitions of n into at most 11 parts.
 * https://oeis.org/A008640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 207, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008640: Number of partitions of n into at most 11 parts */
/* Computed by dynamic programming over parts 1..11. */
/* Complexity: O(11*n) time, O(n) memory. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* the empty partition */
    if (n == 0) {
        return 1;
    }

    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    if (n > max_n) {
        /* Allocate a new table up to n */
        seq_t *newdp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!newdp) {
            /* allocation failure – give up */
            return 0;
        }

        /* initialise */
        for (idx_t i = 0; i <= n; ++i) {
            newdp[i] = 0;
        }
        newdp[0] = 1;  /* a(0) */

        /* dynamic programming: partitions with parts 1..11 */
        for (idx_t part = 1; part <= 11; ++part) {
            for (idx_t i = part; i <= n; ++i) {
                newdp[i] = newdp[i] + newdp[i - part];
            }
        }

        /* replace old table */
        free(dp);
        dp = newdp;
        max_n = n;
    }

    return dp[n];
}
