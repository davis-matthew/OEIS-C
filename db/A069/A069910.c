/*
 * A069910  Expansion of Product_{i in A069908} 1/(1 - x^i).
 * https://oeis.org/A069910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2*n) in ∏_{k odd} (1 + x^k) (number of
 *            partitions of 2*n into distinct odd parts)
 * Range:     u8 n <= 31, u16 n <= 82, u32 n <= 243, u64 n <= 802,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cache of computed a(n) values, up to current maximum n */
static seq_t *a_cache = NULL;
static idx_t cache_len = 0;

/* Forward declaration of helper to compute up to a given n */
static void compute_upto(idx_t n)
{
    idx_t sums = 2 * n;                 /* maximum exponent needed */
    seq_t *dp = (seq_t *)calloc(sums + 1, sizeof(seq_t));
    dp[0] = 1;                          /* empty partition */

    /* Distinct odd parts: product over odd k */
    for (idx_t coin = 1; coin <= sums; coin += 2)
    {
        for (idx_t s = sums; s >= coin; --s)
        {
            dp[s] += dp[s - coin];
        }
    }

    /* Store results in cache */
    seq_t *new_cache = (seq_t *)calloc(n + 1, sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = dp[2 * i];

    /* Free temporary dp array */
    free(dp);

    /* Replace old cache */
    free(a_cache);
    a_cache = new_cache;
    cache_len = n + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (cache_len <= n)
        compute_upto(n);               /* extend cache if necessary */

    return a_cache[n];
}
