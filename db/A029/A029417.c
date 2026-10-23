/*
 * A029417  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A029417: number of partitions of n into parts 6, 7, 9 and 10
    Computation: dynamic programming with the fixed part set {6,7,9,10}.
    dp[i] = number of partitions of i using the allowed parts.  
    For each part p, and for i from p to n, add dp[i-p] to dp[i].
    Complexity: O(n) time and O(n) memory per call.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1.  n is unsigned, cast to size_t. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;               /* allocation failed – unlikely */
    
    /* Initialise dp[0] = 1, others 0 */
    for (size_t i = 1; i < sz; ++i) dp[i] = 0;
    dp[0] = 1;
    
    /* Parts allowed */
    const seq_t parts[4] = {6, 7, 9, 10};
    
    /* Dynamic programming over the parts */
    for (int pi = 0; pi < 4; ++pi) {
        seq_t p = parts[pi];
        for (size_t i = (size_t)p; i < sz; ++i) {
            dp[i] += dp[i - (size_t)p];
        }
    }
    
    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
