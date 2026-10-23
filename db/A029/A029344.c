/*
 * A029344  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029344: number of partitions of n into parts 4, 5, 10, 11.
   Computation is via 1‑dimensional coin‑change DP.  For an index n
   (offset 0), a(n) = number of ways to write n as a sum of 4,5,10,11
   (order of addends irrelevant). */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* allocate an array of size n+1 */
    size_t len = (size_t)n + 1;
    seq_t *dp = alloc_seq_array(len);
    if (!dp)          /* allocation failure: return 0 (sanity) */
        return 0;

    /* initialise */
    dp[0] = 1;

    /* coin denominations */
    static const unsigned int COINS[4] = {4, 5, 10, 11};

    /* DP – add contributions of each coin */
    for (int ci = 0; ci < 4; ++ci)
    {
        unsigned int c = COINS[ci];
        for (size_t i = c; i < len; ++i)
            dp[i] += dp[i - c];
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
