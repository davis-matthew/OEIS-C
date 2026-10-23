/*
 * A029322  Expansion of 1/((1-x^3)*(1-x^9)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of partitions of n into parts 3, 9, 10, and 11.
   Computed by dynamic programming using the standard coin‑change
   recurrence.  dp[i] = number of ways to reach i using the parts
   {3,9,10,11}.  For i < 0 dp[i] = 0; dp[0] = 1.  Complexity: O(n),
   with a small constant factor.  Works for any seq_t type. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate temporary array for dynamic programming. */
    seq_t *dp = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp)
        abort();                   /* out‑of‑memory */

    dp[0] = (seq_t)1;              /* base case */

    /* The distinct part sizes. */
    static const idx_t parts[] = {3, 9, 10, 11};
    const size_t P = sizeof(parts)/sizeof(parts[0]);

    /* Standard coin‑change DP: for each part, add it to all reachable sums. */
    for (size_t p = 0; p < P; ++p) {
        idx_t part = parts[p];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
