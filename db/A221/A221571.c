/*
 * A221571  Number of 0..6 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 0..6 arrays of length n with each element differing *
 *            from at least one neighbor by something other than 1. * The
 *            sequence is computed by dynamic programming over the last two *
 *            symbols. Complexity: O(n) time, O(1) memory
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(1) = 0, n=0 unused */

    enum { ALPHA = 7 };
    seq_t dp[ALPHA][ALPHA];
    seq_t newdp[ALPHA][ALPHA];

    /* initialise dp for length 2: only pairs with non‑adjacent symbols
       satisfy the endpoint rule */
    for (int i = 0; i < ALPHA; ++i)
        for (int j = 0; j < ALPHA; ++j)
            dp[i][j] = (abs(i - j) != 1) ? (seq_t)1 : (seq_t)0;

    /* extend the sequences for lengths 3 .. n */
    for (size_t len = 3; len <= (size_t)n; ++len)
    {
        /* reset newdp */
        for (int i = 0; i < ALPHA; ++i)
            for (int j = 0; j < ALPHA; ++j)
                newdp[i][j] = (seq_t)0;

        for (int prev = 0; prev < ALPHA; ++prev)
            for (int curr = 0; curr < ALPHA; ++curr)
            {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;

                for (int nxt = 0; nxt < ALPHA; ++nxt)
                {
                    /* curr is an interior position; it may not have both
                       neighbours at distance 1 */
                    if ((abs(curr - prev) == 1) && (abs(curr - nxt) == 1))
                        continue;

                    newdp[curr][nxt] += ways;
                }
            }

        /* move newdp into dp */
        for (int i = 0; i < ALPHA; ++i)
            for (int j = 0; j < ALPHA; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* count sequences whose last element also satisfies the endpoint
       rule */
    seq_t total = (seq_t)0;
    for (int prev = 0; prev < ALPHA; ++prev)
        for (int curr = 0; curr < ALPHA; ++curr)
            if (abs(prev - curr) != 1)
                total += dp[prev][curr];

    return total;
}
