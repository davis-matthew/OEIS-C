/*
 * A196933  Column 9 of array A195825. Also column 1 of triangle A195843. Also
 *          1 together with the row sums of triangle A195843.
 * https://oeis.org/A196933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 163, u32 n <= 475, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A196933: number of partitions of n into parts ≡ 0, 1 or 10 (mod 11) */
/* The function uses dynamic programming:  a(n) = Σ a(n‑p) over all allowed parts p ≤ n. */
/* It memoises computed values up to the largest requested n. */

#include <stdlib.h>   /* malloc, free */

/* External types defined by the build system */
typedef uint64_t idx_t;   /* index type */

static seq_t *dp          = NULL;  /* dp[k] = a(k) for 0 ≤ k ≤ max_computed */
static idx_t  max_computed = 0;   /* largest n that has been computed */

/* --------------------------------------------------------------------- */
/* Compute the n‑th term of A196933                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------- */
    /* First call: allocate the table for the requested n.              */
    /* --------------------------------------------------------------- */
    if (dp == NULL) {
        dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;  /* allocation error */

        /* initialise all entries to 0 */
        for (size_t i = 0; i <= n; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;                /* one empty partition */

        /* dynamic program: add each allowed part */
        for (idx_t p = 1; p <= n; ++p) {
            idx_t r = p % (idx_t)11;
            if (r == 0 || r == 1 || r == (idx_t)10) {
                for (size_t j = p; j <= n; ++j)
                    dp[j] += dp[j - (size_t)p];
            }
        }

        max_computed = n;
        return dp[(size_t)n];
    }

    /* --------------------------------------------------------------- */
    /* Subsequent calls: return cached result if available.             */
    /* --------------------------------------------------------------- */
    if (n <= max_computed)
        return dp[(size_t)n];

    /* --------------------------------------------------------------- */
    /* Need to extend the table to accommodate a larger n.            */
    /* Allocate a new array, compute from scratch, free the old array. */
    /* --------------------------------------------------------------- */
    seq_t *new_dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (new_dp == NULL) return (seq_t)0;  /* allocation error */

    for (size_t i = 0; i <= n; ++i)
        new_dp[i] = (seq_t)0;
    new_dp[0] = (seq_t)1;

    for (idx_t p = 1; p <= n; ++p) {
        idx_t r = p % (idx_t)11;
        if (r == 0 || r == 1 || r == (idx_t)10) {
            for (size_t j = p; j <= n; ++j)
                new_dp[j] += new_dp[j - (size_t)p];
        }
    }

    /* free the old array if it was allocated */
    free(dp);

    dp            = new_dp;
    max_computed  = n;
    return dp[(size_t)n];
}
