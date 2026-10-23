/*
 * A161239  Number of partitions of n into Catalan numbers A000108 where every
 *          part appears at least 14 times.
 * https://oeis.org/A161239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into Catalan numbers * where
 *            every part that appears in the partition * appears at least 14
 *            times. The sequence is * computed by dynamic programming on the
 *            coin * values that are Catalan numbers Ck. * * Method: * For
 *            each Catalan value c (unique values only): * 1....
 * Range:     u8 n <= 165, u16 n <= 636, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_one.h"

#define MAX_N 10000

/* static storage for the dynamic programming tables */
static seq_t dp[MAX_N + 1];
static seq_t dp2[MAX_N + 1];
static idx_t computed_up_to = 0;

/* clear tables up to a given index */
static void clear_dp(idx_t upto)
{
    for (idx_t i = 0; i <= upto; ++i)
        dp[i] = 0;
}

static void clear_dp2(idx_t upto)
{
    for (idx_t i = 0; i <= upto; ++i)
        dp2[i] = 0;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* offset is 1 */

    if (n > computed_up_to)
    {
        if (n > MAX_N)
        {   /* Allocate larger tables if needed */
            /* In this simple implementation we assume n <= MAX_N
             * as per the problem description.  If larger n is
             * requested, the function simply returns 0. */
            return 0;
        }

        /* initialise tables */
        clear_dp(n);
        clear_dp2(n);
        dp[0] = seq_one();

        /* generate distinct Catalan numbers <= n */
        const size_t MAX_CAT = 20;
        idx_t cats[MAX_CAT];
        size_t cat_cnt = 0;

        unsigned long long c_val = 1;      /* C0 */
        size_t k = 0;
        idx_t last = (idx_t)-1;

        while (c_val <= (unsigned long long)n)
        {
            idx_t val = (idx_t)c_val;
            if (val != last)
            {
                cats[cat_cnt++] = val;
                last = val;
            }

            /* next Catalan number C_{k+1} = C_k * 2*(2k+1)/(k+2) */
            k++;
            unsigned long long num = c_val * (2ULL * (2ULL * (k - 1) + 1ULL));
            unsigned long long den = (k + 1ULL);
            c_val = num / den;
        }

        /* process each coin (Catalan value) */
        for (size_t ci = 0; ci < cat_cnt; ++ci)
        {
            idx_t coin = cats[ci];

            /* reset temporary table */
            clear_dp2(n);

            /* base addition: 14 copies of the coin */
            for (idx_t s = 0; s <= n; ++s)
            {
                idx_t t = s + 14UL * coin;
                if (t <= n)
                    dp2[t] = dp2[t] + dp[s];
            }

            /* unlimited additional copies of the coin */
            for (idx_t s = 0; s + coin <= n; ++s)
            {
                dp2[s + coin] = dp2[s + coin] + dp2[s];
            }

            /* merge back into dp */
            for (idx_t s = 0; s <= n; ++s)
            {
                dp[s] = dp[s] + dp2[s];
            }
        }

        computed_up_to = n;
    }

    return dp[n];
}
