/*
 * A035945  Number of partitions of n into parts not of the form 11k, 11k+2 or
 *          11k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 4 are greater than 1.
 * https://oeis.org/A035945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of partitions of n into parts not congruent to 0, 2, or 9
 *            modulo 11 (equivalently parts ≠ 11k, 11k+2, 11k-2)
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count partitions with allowed part sizes using dynamic
   programming.  Memoize internally so that multiple calls
   are efficient. */
seq_t a(idx_t n)
{
    static seq_t *dp = NULL;       /* dp[i] = number of partitions of i */
    static idx_t  max_n = 0;      /* largest n for which dp[] is valid */

    /* The sequence is defined for n ≥ 1; return 0 for n = 0. */
    if (n == 0) return 0;

    /* Return a cached value if we already computed it. */
    if (n <= max_n) return dp[n];

    /* Resize the DP array to accommodate up to n. */
    seq_t *newdp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (!newdp) return 0;          /* allocation failure */
    dp = newdp;

    /* Initialise dp[0] = 1 (empty partition), others to 0. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Standard coin‑change DP over allowed part sizes. */
    for (idx_t part = 1; part <= n; ++part) {
        idx_t r = part % 11;
        if (r == 0 || r == 2 || r == 9) continue;   /* forbidden residues */
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    max_n = n;
    return dp[n];
}
