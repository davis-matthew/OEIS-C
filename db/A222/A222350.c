/*
 * A222350  Number of (n+6)X1 arrays of occupancy after each element moves up
 *          to +-n places including 0.
 * https://oeis.org/A222350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of occupancy arrays of length n+6 obtainable
 *            * by letting each of the n+6 elements move at most n positions
 *            left or * right (including staying). The sequence has offset 1
 *            and is defined for all * n ≥ 1. The count can be written as * *
 *            a(n) = Σ_{s=0}^{n+5} Σ_{t=0}^{n+5} Lleft[s]...
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   binom, dfs_brute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard C headers */
#include <stdlib.h>
#include <stdint.h>
/* Utility for binomial coefficients */
#include "../utils/binom.h"
#include "../utils/dfs_brute.h"

/* ------------------------------------------------------------------------ */
/* Helper for n ≤ 4: brute‑force enumeration of all admissible occupancy   */
/* vectors.  Recursively assign values to each of the L = n+6 positions.    */

/* ------------------------------------------------------------------------ */
/* Main sequence generation routine                                       */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --------  Small n (≤ 4) – brute force -------------------------------- */
    if (n <= 4) {
        idx_t L = n + 6;
        idx_t  arr[20];                     /* L ≤ 10 for n ≤ 4          */
        seq_t  counter = 0;
        dfs_brute(0, L, 0, arr, n, L, &counter);
        return counter;
    }

    /* --------  n > 4 – use analytic formula -------------------------------- */
    idx_t L   = n + 6;                     /* total length of array      */
    idx_t M   = n - 4;                     /* number of middle (unconstrained) cells */
    size_t maxSum = (size_t)(n + 5);        /* sums that may appear on each side */

    /* Allocate arrays for DP over the first five positions (prefix side). */
    size_t size = maxSum + 2;              /* allow index up to n+5         */
    seq_t *dp  = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *next= (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp || !next) {                   /* In case of allocation failure */
        if (dp) free(dp);
        if (next) free(next);
        return 0;
    }

    dp[0] = 1;                             /* initial empty configuration  */

    /* DP over the first five cells, respecting their upper bounds.      */
    for (int j = 0; j < 5; ++j) {
        size_t U_curr = (size_t)(n + j);    /* current maximum achievable sum */
        size_t U_next = U_curr + 1;         /* after adding the next cell     */

        seq_t sum = 0;
        for (size_t s = 0; s <= U_next; ++s) {
            if (s <= U_curr)
                sum += dp[s];
            next[s] = sum;
        }
        /* Copy back to dp for the next iteration.                          */
        for (size_t s = 0; s <= U_next; ++s)
            dp[s] = next[s];
    }

    /* dp[s] now holds the number of ways to populate the first five cells
     * with a total of s elements, subject to the prefix constraints. */
    /* Pre‑compute binomial coefficients C(L - k + M - 1 , M - 1).          */
    size_t combSize = (size_t)L + 1;
    seq_t *comb = (seq_t *)calloc(combSize, sizeof(seq_t));
    for (size_t k = 0; k <= L; ++k) {
        idx_t R = L - k;                   /* elements left for the middle etc. */
        if (R >= 0)
            comb[k] = binom((idx_t)(R + M - 1), (idx_t)(M - 1));
        else
            comb[k] = 0;
    }

    /* Convolution of the left and right side counts, weighted by the
     * corresponding binomial term.                                            */
    seq_t result = 0;
    for (size_t s = 0; s <= maxSum; ++s) {
        seq_t leftWays = dp[s];
        if (leftWays == 0) continue;
        for (size_t t = 0; t <= maxSum; ++t) {
            seq_t rightWays = dp[t];
            if (rightWays == 0) continue;
            size_t k = s + t;
            if (k > L) continue;           /* remainder would be negative   */
            seq_t term = leftWays * rightWays;
            term = term * comb[k];
            result += term;
        }
    }

    /* Free allocated memory.                                                      */
    free(dp);
    free(next);
    free(comb);

    return result;
}
