/*
 * A117957  Number of partitions of n into parts larger than 1 and congruent
 *          to 1 mod 4.
 * https://oeis.org/A117957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117957: number of partitions of n into parts >1 congruent to 1 (mod 4).
   The count is obtained by a simple dynamic‑programming
     dp[0] = 1
   and for each possible part p = 5,9,13,…≤n:
       for s = p … n
           dp[s] += dp[s–p]
   The value a(n) is dp[n].  This implements the generating function
   1 / ∏(1 – x^(4i+1)), i≥1.
*/

#include <stdlib.h>

/* Returns the number of such partitions for the given non‑negative n.  */
seq_t a(idx_t n)
{
    /* Allocate an array of size n+1.  The caller supplies a suitable
       seq_t type that overloads addition.  */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;          /* allocation failed – return 0 */

    /* Initialise all entries to 0 and set the base case.  */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Dynamic programming: add each allowed part.  */
    for (idx_t part = 5; part <= n; part += 4) {
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] = dp[sum] + dp[sum - part];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
