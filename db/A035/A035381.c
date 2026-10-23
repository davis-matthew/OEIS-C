/*
 * A035381  Number of partitions of n into parts 6k+1 or 6k+3.
 * https://oeis.org/A035381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A035381:
 * a(n) = number of partitions of n into parts ≡1 or 3 (mod 6). */
/* It uses dynamic programming (unbounded coin change). */

#include <stdlib.h>   /* malloc, free, calloc */
#include <string.h>   /* memset */

/* External integer types supplied by the build system */
extern seq_t a(idx_t n);

/* Public function */
seq_t a(idx_t n)
{
    /* Offset is 1: for n < 1, sequence is undefined.  Return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Allocate zero‑initialized array of size n+1.  Use calloc for safety. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* allocation failure, return 0 */

    dp[0] = (seq_t)1;  /* base case: one way to make sum 0 */

    /* Iterate over all allowed part sizes (1 or 3 mod 6). */
    for (idx_t part = 1; part <= n; ++part) {
        idx_t mod = part % 6;
        if (mod != 1 && mod != 3)
            continue;          /* skip disallowed parts */

        /* Inner loop: add ways to reach sums that include this part. */
        for (idx_t j = part; j <= n; ++j)
            dp[j] += dp[j - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
