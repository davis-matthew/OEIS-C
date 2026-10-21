/*
 * A212548  Number of partitions of n containing at least one part m-8 if m is
 *          the largest part.
 * https://oeis.org/A212548
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=9}^{⌊(n+8)/2⌋} p(n−2m+8, m), where p(s, m) counts
 *            partitions of s with parts ≤ m
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 143, u64 n <= 457,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* maximum n expected in the tests – safe upper bound */
#define MAX_N 10000u
#define MAX_M ((MAX_N + 8u) / 2u)

/* tables of the sequence and of partitions counted with largest part ≤ current j */
static seq_t  ans[MAX_N + 1];
static seq_t  dp [MAX_N + 1];
static bool   init = false;

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* values below the offset are undefined – return 0 */
    if (n < 8u || n > MAX_N)
        return 0;

    if (!init)
    {
        /* initialise partition table */
        dp[0] = 1;            /* empty partition */

        /* iterate over all possible parts j = 1 … MAX_M */
        for (idx_t j = 1u; j <= MAX_M; ++j)
        {
            /* propagate partitions using part size j */
            for (idx_t s = j; s <= MAX_N; ++s)
                dp[s] += dp[s - j];

            /* for j ≥ 9 we have a valid largest part m = j */
            if (j >= 9u)
            {
                const idx_t offset = (2u * j) - 8u;      /* n = r + offset */
                const idx_t r_max  = MAX_N - offset;

                for (idx_t r = 0u; r <= r_max; ++r)
                    ans[r + offset] += dp[r];
            }
        }

        init = true;
    }

    return ans[n];
}
