/*
 * A183959  Number of strings of numbers x(i=1..8) in 0..n with sum i^2*x(i)
 *          equal to n*64.
 * https://oeis.org/A183959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to Σ_{i=1}^8 (i^2)*x(i) =
 *            64*n with 0 ≤ x(i) ≤ n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 48, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Target sum is 64*n */
    idx_t target = n * (idx_t)64;
    /* Allocate DP arrays; use explicit cast from void* to seq_t* for C/C++ */
    size_t maxSum = (size_t)target;
    seq_t *dp   = (seq_t*)calloc(maxSum + 1, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(maxSum + 1, sizeof(seq_t));
    if (!dp || !next) {
        /* Allocation failed – return 0 as a saf­e fallback */
        free(dp);
        free(next);
        return 0;
    }

    /* Initially, only sum 0 is achievable */
    dp[0] = 1;

    /* Process each variable x(i) with weight w = i^2,
       bounded by 0..n (exactly n+1 possible values) */
    for (idx_t vi = 1; vi <= 8; ++vi) {
        size_t w = (size_t)(vi * vi);        /* weight of this variable */
        /* For every residue class modulo w */
        for (size_t rem = 0; rem < w; ++rem) {
            seq_t window = 0;                /* sum of the last n+1 terms */
            for (size_t pos = rem, step = 0; pos <= maxSum; ++step, pos += w) {
                window += dp[pos];           /* include new term */
                if (step > (size_t)n) {      /* drop term that leaves window */
                    size_t oldPos = rem + (step - n - 1) * w;
                    window -= dp[oldPos];
                }
                next[pos] = window;           /* store new DP value */
            }
        }
        /* Prepare for next variable */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[target];
    free(dp);
    free(next);
    return result;
}
