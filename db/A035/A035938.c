/*
 * A035938  Number of partitions in parts not of the form 7k, 7k+2 or 7k-2.
 *          Also number of partitions with 1 part of size 1 and differences
 *          between parts at distance 2 are greater than 1.
 * https://oeis.org/A035938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   partitions into parts congruent to 1,3,4,6 modulo 7 (i.e. not of
 *            the form 7k, 7k+2 or 7k-2)
 * Range:     u8 n <= 25, u16 n <= 68, u32 n <= 208, u64 n <= 693,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ compiler – use std::vector which correctly constructs
       arbitrary‑precision seq_t objects. */
    std::vector<seq_t> dp(n + 1, 0);
    dp[0] = 1;

    for (idx_t m = 1; m <= n; ++m)
    {
        if (m % 7 == 0 || m % 7 == 2 || m % 7 == 5) continue; /* forbidden residue */
        for (idx_t s = m; s <= n; ++s)
            dp[s] += dp[s - m];
    }
    return dp[n];
#else
    /* C compiler – seq_t is a plain integer type, so we can use
       memory allocation with calloc. */
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp) return 0;                /* allocation failure → 0 as safe default */
    dp[0] = 1;

    for (idx_t m = 1; m <= n; ++m)
    {
        if (m % 7 == 0 || m % 7 == 2 || m % 7 == 5) continue; /* forbidden residue */
        for (idx_t s = m; s <= n; ++s)
            dp[s] += dp[s - m];
    }
    seq_t result = dp[n];
    free(dp);
    return result;
#endif
}
