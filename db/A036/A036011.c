/*
 * A036011  Number of partitions of n into parts not of the form 25k, 25k+12
 *          or 25k-12. Also number of partitions with at most 11 parts of size
 *          1 and differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not of the form 25k,
 *            25k+12, or 25k-12. Computed by dynamic programming over all
 *            allowed part sizes
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 135, u64 n <= 449,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t dp_max = (idx_t)-1;          /* no data yet */

static void recompute_dp(idx_t n)
{
    /* allocate new array for partitions up to n */
    seq_t *newdp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!newdp) return;   /* allocation failed – unlikely */

    newdp[0] = 1;
    for (idx_t part = 1; part <= n; ++part) {
        seq_t r = part % 25;
        if (r == 0 || r == 12 || r == 13) continue; /* forbidden residue */
        for (idx_t i = part; i <= n; ++i)
            newdp[i] += newdp[i - part];
    }

    if (dp) free(dp);
    dp      = newdp;
    dp_max  = n;
}

seq_t a(idx_t n)
{
    if (!dp || n > dp_max) recompute_dp(n);
    return dp ? dp[n] : (seq_t)0;
}
