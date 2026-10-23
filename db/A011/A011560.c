/*
 * A011560  Stirling numbers of second kind S2(11,n).
 * https://oeis.org/A011560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = S(11,n) - Stirling numbers of the second kind for n=11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t cache[12] = {0};
    static bool init = false;

    if (!init)
    {
        /* compute S(11,k) for k=0..11 */
        seq_t dp[12] = {0};
        dp[0] = 1;  /* S(0,0) = 1 */

        for (idx_t i = 1; i <= 11; ++i)
        {
            seq_t next[12] = {0};
            for (idx_t k = 1; k <= i; ++k)
            {
                /* next[k] = k * dp[k] + dp[k-1] */
                next[k] = (seq_t)k * dp[k] + dp[k - 1];
            }
            /* copy next into dp for next iteration */
            for (idx_t k = 0; k <= 11; ++k)
                dp[k] = next[k];
        }

        /* Store results in cache[1..11] */
        for (idx_t k = 1; k <= 11; ++k)
            cache[k] = dp[k];

        init = true;
    }

    /* If n > 11, the Stirling number S(11,n) is 0 */
    if (n > 11 || n == 0)
        return 0;

    return cache[n];
}
