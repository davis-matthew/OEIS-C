/*
 * A035975  Number of partitions of n into parts not of the form 19k, 19k+6 or
 *          19k-6. Also number of partitions with at most 5 parts of size 1
 *          and differences between parts at distance 8 are greater than 1.
 * https://oeis.org/A035975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into parts not
   congruent to 0, 6, or 13 modulo 19.
   DP: dp[0] = 1; for each allowed part p,
   for i = p..n dp[i] += dp[i-p].
   The table is grown lazily; only the largest n seen is recomputed.
   Valid C11 and C++17. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"

/* Cached partition table */
static seq_t *dp = NULL;          /* array of length max_computed+1 */
static idx_t  max_computed = 0;   /* largest n for which dp has been filled */

/* Recompute dp up to (and including) n */
static void recompute(idx_t n)
{
    if (n <= max_computed) return;

    /* Allocate new table, zero‑inited by alloc_seq */
    seq_t *newdp = alloc_seq((size_t)(n + 1));
    if (!newdp) return;          /* allocation failure – return without changing cache */

    /* Initialise to zero (alloc_seq may not zero-initialize) */
    for (idx_t i = 0; i <= n; ++i) newdp[i] = 0;
    newdp[0] = 1;                /* one partition of 0 */

    /* Dynamic programming over allowed part sizes */
    for (idx_t part = 1; part <= n; ++part)
    {
        /* Skip forbidden residues modulo 19 */
        if (part % 19 == 0 || part % 19 == 6 || part % 19 == 13)
            continue;

        for (idx_t sum = part; sum <= n; ++sum)
            newdp[sum] += newdp[sum - part];
    }

    /* Replace old table */
    free(dp);
    dp = newdp;
    max_computed = n;
}

/* Public accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* the sequence is defined for n ≥ 1 */

    /* Grow cache if required */
    if (n > max_computed)
        recompute(n);

    return dp[n];
}
