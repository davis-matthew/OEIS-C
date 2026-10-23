/*
 * A029078  Expansion of 1/((1-x)*(1-x^4)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029078
   a(n) = number of partitions of n into the parts 1, 4, 8, 9.
   The function uses a simple dynamic‑programming tabulation.
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp_cache = NULL;  /* cache for computed values */
static idx_t  cached_max = 0;   /* largest index currently stored */

/* Compute dp[0..n] into dp_cache, overwriting any previous contents. */
static void compute_dp(idx_t n)
{
    size_t size = (size_t)(n + 1);           /* array length: 0 .. n */
    seq_t *new_cache = (seq_t*)malloc(size * sizeof(seq_t));
    if (!new_cache)
        abort();                             /* allocation failure */

    /* initialise all entries to 0 */
    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = 0;

    new_cache[0] = 1;                        /* empty partition */

    /* parts allowed in the partitions */
    const idx_t parts[4] = {1, 4, 8, 9};

    /* dynamic‑programming:  for each part, add its contributions */
    for (int k = 0; k < 4; ++k)
    {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i)
            new_cache[i] += new_cache[i - p];
    }

    /* Replace old cache with new one */
    if (dp_cache)
        free(dp_cache);
    dp_cache    = new_cache;
    cached_max  = n;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (!dp_cache || n > cached_max)
        compute_dp(n);
    return dp_cache[n];
}
