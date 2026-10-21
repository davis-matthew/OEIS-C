/*
 * A097242  Expansion of q-series 1 / (q^2, q^3, q^9, q^10; q^12)_infinity.
 * https://oeis.org/A097242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 124, u32 n <= 367, u64 n <= 1207,
 *            u128 n <= 4268, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A097242: partitions into distinct parts not congruent to 1 or 5 modulo 6.
   Generating function: ∏_{k>0} (1 + x^(6k-3)) / (1 - x^(4k-2))
   The DP below uses the equivalent interpretation:
   a(n) = number of partitions of n into distinct parts with parts ∈ {m : m%6≠1 && m%6≠5}.
   Pack the DP with static caching that is refreshed whenever a larger `n` is requested.
*/

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* static cache for the DP table */
static seq_t *dp = NULL;       /* dp[i] = a(i) for 0 <= i < dp_size */
static size_t dp_size = 0;     /* number of slots in dp (== last_computed_n + 1) */

/* recompute the whole table up to (and including) `n` */
static void compute_dp(idx_t n)
{
    size_t size = (size_t)n + 1;
    seq_t *newdp = alloc_seq_array(size);   /* zeros are already set by the helper */
    newdp[0] = (seq_t)1;                    /* a(0) = 1 */

    /* iterate over all allowed part sizes: every integer p ≥ 2 with p%6≠1 and p%6≠5 */
    for (idx_t p = 2; p <= n; ++p) {
        idx_t r = p % 6;
        if (r == 1 || r == 5) continue;   /* forbidden residues */
        for (idx_t i = n; i >= p; --i) {  /* distinct parts ⇒ reverse loop */
            newdp[i] += newdp[i - p];
        }
    }

    /* replace old table */
    free(dp);
    dp      = newdp;
    dp_size = size;
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    if ((size_t)n + 1 > dp_size)      /* need a larger table */
        compute_dp(n);
    return dp[(size_t)n];
}
