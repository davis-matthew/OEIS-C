/*
 * A103925  Number of partitions of n into parts but with two kinds of parts
 *          of sizes 1,2,3,4,5 and 6.
 * https://oeis.org/A103925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts with two kinds of
 *            parts of sizes 1,2,3,4,5,6 (size k≤6 has two colors), i.e.
 *            generating function ∏_{k=1..6} 1/(1−x^k)^2 ∏_{j=7..∞} 1/(1−x^j)
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 75, u64 n <= 274,
 *            u128 n <= 1083, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *dp = alloc_seq_array(sz);
    if (!dp) return (seq_t)0;          /* allocation failed */

    /* Initialise to zero */
    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;

    /* For each part size from 1 to n, process its colour(s) */
    for (idx_t size = 1; size <= n; ++size)
    {
        int times = (size <= (idx_t)6) ? 2 : 1;   /* two colours for sizes ≤6 */
        for (int t = 0; t < times; ++t)
        {
            for (idx_t s = size; s <= n; ++s)
            {
                /* dp[s] += dp[s - size] */
                dp[(size_t)s] += dp[(size_t)(s - size)];
            }
        }
    }

    return dp[(size_t)n];
}
