/*
 * A132462  Number of partitions of n into distinct parts congruent to 0 or 2
 *          modulo 3.
 * https://oeis.org/A132462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 125, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   alloc_seq_array, free_seq_array, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A132462:  number of partitions of n into distinct parts 
 *  congruent to 0 or 2 modulo 3.
 *  We compute it by standard 1‑dimensional dynamic programming.
 *  For every allowed part p (p%3==0 or 2) we add dp[s‑p] to dp[s]
 *  for all s≥p, iterating s downward to keep the parts distinct.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/seq_one.h"

/* Memoised DP table so repeated calls are fast */
static seq_t *dp      = NULL;   /* dp[s] = a(s) for s currently computed  */
static size_t dp_cap  = 0;      /* array capacity (largest index + 1)      */

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Resize (or initialise) the DP array up to n */
    if (dp_cap < (size_t)(n + 1)) {
        seq_t *newdp = alloc_seq_array((size_t)(n + 1));
        /* copy old values if any */
        for (size_t i = 0; i < dp_cap; ++i) newdp[i] = dp[i];
        /* zero new entries */
        for (size_t i = dp_cap; i <= (size_t)n; ++i) newdp[i] = 0;
        if (dp != NULL) free_seq_array(dp);
        dp = newdp;
        dp_cap = (size_t)(n + 1);
    }

    /* Reset dp[0] and all other indices up to n */
    dp[0] = seq_one();
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* 1‑dimensional DP */
    for (idx_t p = 2; p <= n; ++p) {
        if (p % 3 == 0 || p % 3 == 2) {
            for (idx_t s = n; s >= p; --s) {
                dp[s] = dp[s] + dp[s - p];
            }
        }
    }
    return dp[n];
}
