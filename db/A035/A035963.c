/*
 * A035963  Number of partitions of n into parts not of the form 17k, 17k+2 or
 *          17k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 7 are greater than 1.
 * https://oeis.org/A035963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A035963
 *  a(n) = number of partitions of n into parts not congruent to 0, 2 or -2 (≡15) mod 17.
 *
 *  The sequence is computed by dynamic programming over the allowed part sizes:
 *      Let DP[0] = 1 and DP[k] = 0 for k > 0.
 *      For each part p with p mod 17 ∉ {0, 2, 15}:
 *          for (i = p; i <= n; ++i)
 *              DP[i] += DP[i - p];
 *
 *  Complexity:  O(n²) additions on the type seq_t.  For n = 10 000 that is ≈41 × 10⁶
 *  operations, easily within the execution limits.
 *
 *  The implementation uses a static cache so that multiple calls with any n
 *  are served efficiently.
 */

#include <stdlib.h>

/* Assume seq_t and idx_t are provided by the build environment.           */
seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *dp = NULL;
    static idx_t cache_max = 0;  /* greatest n that has been computed */

    /* If we already have a value for this n, return it. */
    if (n <= cache_max)
        return dp[n];

    /* Allocate or re‑allocate the DP array for the new maximum size. */
    if (dp != NULL)
        free(dp);
    dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (dp == NULL)  /* allocation failure – abort gracefully */
        return (seq_t)0;

    /* Initialise the DP array.  DP[0] = 1 (empty partition). */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = 1;

    /* Main DP loop over all allowed part sizes.          */
    for (idx_t part = 1; part <= n; ++part) {
        /* Skip parts that are multiples of 17 or congruent to ±2 mod 17. */
        idx_t r = part % 17;
        if (r == 0 || r == 2 || r == 15)
            continue;

        /* Add this part to all higher indices. */
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    cache_max = n;
    return dp[n];
}
