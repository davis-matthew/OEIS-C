/*
 * A116371  Number of partitions of n into parts with digital root = 1.
 * https://oeis.org/A116371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into parts with digital root =
 *            1
 * Range:     u8 n <= 114, u16 n <= 320, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *dp = NULL;        /* number of partitions for each value up to the cached maximum */
static idx_t max_cached = 0;    /* largest n for which dp[0..n] are valid */

/* Recompute dp[0..n] from scratch when a larger n is requested */
static void recompute_upto(idx_t n)
{
    if (n <= max_cached)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!tmp)                    /* memory allocation failure */
        abort();
    dp = tmp;

    dp[0] = (seq_t)1;            /* base case: one way to sum to 0 */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;               /* initialise the rest to zero */

    /* Process each part that is congruent to 1 modulo 9 */
    for (idx_t part = 1; part <= n; part += 9)
    {
        for (idx_t s = part; s <= n; ++s)
            dp[s] += dp[s - part];
    }

    max_cached = n;
}

/* Public function required by the series entry */
seq_t a(idx_t n)
{
    if (n == 0)                 /* not used for the offset, but harmless */
        return (seq_t)0;
    recompute_upto(n);
    return dp[n];
}
