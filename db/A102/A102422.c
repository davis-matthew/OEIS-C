/*
 * A102422  Number of partitions of n with k <= 5 parts and each part p <= 5.
 * https://oeis.org/A102422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   number of partitions of n with at most 5 parts, each part ≤5
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static void precompute(seq_t table[26])
{
    enum { NMAX = 25, PARTS = 5 };
    seq_t dp[NMAX + 1][PARTS + 1] = {{0}};   /* dp[sum][cnt] */
    dp[0][0] = 1;                           /* empty partition */

    /* iterate over part values 1..5  (each can be used any number of times) */
    for (int j = 1; j <= PARTS; ++j)
    {
        for (int s = j; s <= NMAX; ++s)
        {
            for (int c = 1; c <= PARTS; ++c)
            {
                dp[s][c] += dp[s - j][c - 1];
            }
        }
    }

    /* build the table of answers */
    for (int n = 0; n <= NMAX; ++n)
    {
        seq_t sum = 0;
        for (int c = 0; c <= PARTS; ++c)
            sum += dp[n][c];
        table[n] = sum;
    }
}

seq_t a(idx_t n)
{
    static seq_t memo[26];
    static bool done = false;

    if (!done)
    {
        precompute(memo);
        done = true;
    }

    if (n > 25)
        return (seq_t)0;
    return memo[(int)n];
}
