/*
 * A035953  Number of partitions of n into parts not of the form 13k, 13k+5 or
 *          13k-5. Also number of partitions with at most 4 parts of size 1
 *          and differences between parts at distance 5 are greater than 1.
 * https://oeis.org/A035953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of partitions of n into parts not ≡ 0,5,8 (mod 13)
   (i.e., allowed part residues are 1,2,3,4,6,7,9,10,11,12).
   Standard partition DP: dp[i] = Σ dp[i‑p] over allowed parts p ≤ i.
   For efficient handling of multiple calls, recompute the DP array only
   when a larger n is requested.  The array dp[0..max] is kept
   between calls so that smaller queries are answered in O(1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* global state */
    static seq_t *dp = NULL;            /* PARTITION COUNTS */
    static idx_t max_computed = 0;      /* largest index fully computed */

    /* request for n < 1 is not expected but return 0 safely */
    if (n == 0) return (seq_t)0;

    /* recompute if we haven't reached this n yet */
    if (n > max_computed) {
        /* allocate or resize the dp array */
        if (dp == NULL) {
            dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        } else {
            dp = (seq_t *)realloc(dp, sizeof(seq_t) * (size_t)(n + 1));
            /* zero the new entries that will be recomputed */
            for (idx_t i = max_computed + 1; i <= n; ++i) dp[i] = 0;
        }
        /* initialise the partition counts */
        dp[0] = (seq_t)1;                 /* empty partition */
        for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

        /* standard partition DP over allowed parts */
        for (idx_t part = 1; part <= n; ++part) {
            if (part % 13 == 0 || part % 13 == 5 || part % 13 == 8)
                continue;                 /* forbidden parts */
            for (idx_t j = part; j <= n; ++j)
                dp[j] += dp[j - part];
        }
        max_computed = n;                 /* update cache state */
    }

    /* dp[n] now contains a(n) */
    return dp[n];
}
