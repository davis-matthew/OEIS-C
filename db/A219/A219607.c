/*
 * A219607  Number of partitions of n into distinct parts 5*k+2 or 5*k+3.
 * https://oeis.org/A219607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts of the form
 *            5*k+2 or 5*k+3 (OEIS A219607). The routine recomputes the
 *            partition table whenever a larger n is requested, using a
 *            classic 0/1 knapsack DP. Each call is O(n^2) in the worst case,
 *            but n≤10 000 leads to well under 2 s
 * Range:     u8 n <= 82, u16 n <= 211, u32 n <= 619, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;  /* dp[i] = a(i) for 0 <= i <= max_n */
    static idx_t max_n = (idx_t)-1;  /* largest n computed so far */

    /* Allocate or grow the dp array to fit n+1 entries. */
    if (dp == NULL || n > max_n)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        dp = tmp;

        /* Zero all entries that will be recomputed. */
        for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
        dp[0] = 1;

        /* Classic 0/1 knapsack over allowed parts. */
        for (idx_t k = 0; ; ++k)
        {
            idx_t p1 = 5 * k + 2;
            idx_t p2 = 5 * k + 3;
            if (p1 > n && p2 > n) break;

            if (p1 <= n)
                for (idx_t i = n; i >= p1; --i)
                    dp[i] += dp[i - p1];

            if (p2 <= n)
                for (idx_t i = n; i >= p2; --i)
                    dp[i] += dp[i - p2];
        }

        max_n = n;
    }

    return dp[n];
}
