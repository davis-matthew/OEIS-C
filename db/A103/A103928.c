/*
 * A103928  Number of partitions of n into parts but with two kinds of parts
 *          of sizes 1 to 9.
 * https://oeis.org/A103928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts, where parts 1…9
 *            each have two kinds, parts ≥10 have one kind. This is
 *            implemented via a standard partition‑DP that handles each part
 *            type as an unlimited coin. The table dp[n] holds a(n)
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;       /* dynamic table of a(n) */
static idx_t  max_n   = 0;     /* largest n already computed */

/* Recompute dp[0…N] using the generating function
   ((∏_{k=1..9} 1/(1-x^k))^2) · (∏_{j≥10} 1/(1-x^j)) . */
static void recompute(idx_t N)
{
    /* allocate a fresh table */
    seq_t *newdp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!newdp) return;                 /* a memory error – leave old table */

    /* initialise to zero */
    for (idx_t i = 0; i <= N; ++i) newdp[i] = 0;
    newdp[0] = 1;                       /* one way to make sum 0 */

    /* parts 1 … 9, each present in two kinds (two independent types) */
    for (idx_t s = 1; s <= 9; ++s) {
        for (int kind = 0; kind < 2; ++kind) {   /* two colours */
            for (idx_t j = s; j <= N; ++j) {
                newdp[j] += newdp[j - s];
            }
        }
    }

    /* parts 10 … N, only one kind for each size */
    for (idx_t s = 10; s <= N; ++s) {
        for (idx_t j = s; j <= N; ++j) {
            newdp[j] += newdp[j - s];
        }
    }

    /* replace the old table */
    free(dp);
    dp     = newdp;
    max_n  = N;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    /* initialise on first use or enlarge if necessary */
    if (dp == NULL || n > max_n) recompute(n);
    return dp[n];
}
