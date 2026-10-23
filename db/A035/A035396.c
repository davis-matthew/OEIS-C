/*
 * A035396  Number of partitions of n into parts 7k+1 or 7k+2.
 * https://oeis.org/A035396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A035396: the number of partitions
   of n into parts of the form 7k+1 or 7k+2.  It uses a simple
   dynamic‑programming approach based on the product generating
   function   ∏_{k≥0} (1−x^{7k+1})⁻¹ (1−x^{7k+2})⁻¹,
   which yields the recurrence
        dp[i] = Σ_{p allowed, p≤i} dp[i−p],
   with dp[0] = 1.  The algorithm runs in O(n²/7) time and
   uses O(n) memory. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */
    size_t len = (size_t)n + 1;           /* array size n+1 */
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return 0;                    /* allocation failure */

    /* initialisation */
    dp[0] = 1;
    for (size_t i = 1; i < len; ++i) dp[i] = 0;

    /* add parts of the form 7k+1 */
    for (idx_t part = 1; part <= n; part += 7)
    {
        size_t p = (size_t)part;
        for (size_t i = p; i < len; ++i)
            dp[i] += dp[i - p];
    }

    /* add parts of the form 7k+2 */
    for (idx_t part = 2; part <= n; part += 7)
    {
        size_t p = (size_t)part;
        for (size_t i = p; i < len; ++i)
            dp[i] += dp[i - p];
    }

    seq_t result = dp[len - 1];
    free(dp);
    return result;
}
