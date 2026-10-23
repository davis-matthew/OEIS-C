/*
 * A105780  Coefficients of the A-Rogers mod 14 identity.
 * https://oeis.org/A105780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 149, u64 n <= 500,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A105780  :  partitions of n into parts ≠ 0,6,8 (mod 14)
 *  The number of partitions of n whose parts avoid the residue classes
 *  0, 6 and 8 modulo 14.  It is computed by an ordinary unbounded‑knapsack
 *  DP:  dp[0] = 1, and for every allowed part size s
 *      for i = s … n   dp[i] += dp[i‑s];
 *  The algorithm runs in O(n²) and is recomputed only when a larger
 *  index is requested.  It works for any seq_t / idx_t definition.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *dp = NULL;      /* array of partition counts */
static idx_t  max_n = 0;      /* highest n that has been computed */

/* recompute all values up to n */
static void
recompute(idx_t n)
{
    /* allocate or reallocate storage */
    dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    /* clear the array */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;                           /* empty partition */

    /* main DP: add all allowed part sizes */
    for (idx_t s = 1; s <= n; ++s) {
        /* skip forbidden residues modulo 14 */
        if (s % 14 == 0 || s % 14 == 6 || s % 14 == 8)
            continue;
        for (idx_t i = s; i <= n; ++i)
            dp[i] += dp[i - s];
    }
    max_n = n;
}

/* public entry point */
seq_t
a(idx_t n)
{
    if (n > max_n || dp == NULL)
        recompute(n);
    return dp[n];
}
