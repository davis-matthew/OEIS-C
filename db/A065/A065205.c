/*
 * A065205  Number of subsets of proper divisors of n that sum to n.
 * https://oeis.org/A065205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of proper divisors of n that sum to n
 * Range:     u8 n <= 119, u16 n <= 719, u32 n <= 2519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

/* Returns the number of subsets of the proper divisors of n
   that sum to n.  Proper divisors are all positive divisors of n
   except n itself.  Each divisor may be used at most once.  */
seq_t a(idx_t n)
{
    /* Collect the proper divisors of n into a small static array. */
    idx_t divs[128];
    size_t divcnt = 0;
    for (idx_t i = 1; (i * i) <= n; ++i)
    {
        if (n % i == 0)
        {
            if (i != n)
                divs[divcnt++] = i;
            idx_t j = n / i;
            if (j != i && j != n)
                divs[divcnt++] = j;
        }
    }

    /* Dynamic array for DP: dp[s] = number of subsets of the
       processed divisors that sum to s.  We re-use the same
       buffer across calls for efficiency. */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    ensure_capacity(n + 1, &dp_cap, (void **)&dp, sizeof(seq_t));

    /* Initialize DP.  dp[0] = 1 (empty set), all others 0. */
    dp[0] = 1;
    for (size_t s = 1; s <= n; ++s)
        dp[s] = 0;

    /* Subset-sum DP over the proper divisors. */
    for (size_t idx = 0; idx < divcnt; ++idx)
    {
        idx_t d = divs[idx];
        for (size_t sum = n; sum >= (size_t)d; --sum)
            dp[sum] = dp[sum] + dp[sum - (size_t)d];
    }

    return dp[n];
}
