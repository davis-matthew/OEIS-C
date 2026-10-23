/*
 * A035944  Number of partitions in parts not of the form 11k, 11k+1 or 11k-1.
 *          Also number of partitions with no part of size 1 and differences
 *          between parts at distance 4 are greater than 1.
 * https://oeis.org/A035944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A035944: number of partitions of n using parts not congruent to 0,1,10 (mod 11)
   (i.e., parts not of the form 11k, 11k+1 or 11k-1).  The function uses a
   standard coin‑change DP: for each allowed part p, for all sums s ≥ p
   add dp[s‑p] to dp[s].  Complexity O(n²) which is fine for n ≤ 10 000.  */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* No meaningful value for n < 1 (offset is 1), but just in case */
    if (n < 1) return 0;

    /* Allocate DP array of size n+1 and clear it */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    dp[0] = 1;                        /* one way to make sum 0 *)

    /* Iterate over all potential part sizes */
    for (size_t p = 2; p <= N; ++p) {
        /* Skip parts of the forbidden congruence classes */
        unsigned rem = (unsigned)(p % 11);
        if (rem == 0 || rem == 1 || rem == 10) continue;

        /* Update DP for this part */
        for (size_t s = p; s <= N; ++s) {
            dp[s] += dp[s - p];
        }
    }

    seq_t res = dp[N];
    free(dp);
    return res;
}
