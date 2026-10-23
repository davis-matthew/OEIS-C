/*
 * A035971  Number of partitions of n into parts not of the form 19k, 19k+2 or
 *          19k-2. Also number of partitions with 1 part of size 1 and
 *          differences between parts at distance 8 are greater than 1.
 * https://oeis.org/A035971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of partitions of n into parts whose size is NOT
   congruent to 0, 2, or 17 modulo 19.  This is equivalent to the
   OEIS sequence A035971.  The routine uses dynamic programming:
      dp[0] = 1
      for part = 1 .. n
          if part % 19 not in {0,2,17}
              for sum = part .. n
                  dp[sum] += dp[sum - part]
   The array is recomputed only when a larger n is requested,
   so repeated calls are cheap.  No floating point arithmetic is used;
   everything is done with integer types (or the arbitrary‑precision
   integer class supplied by the build). */

#include <stdlib.h>

/* static state keeps the current DP cache */
static seq_t *dp = NULL;
static idx_t max_n = 0;

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; return 0 for n=0 to be safe */
        return 0;

    if (n > max_n) {
        /* Allocate new DP array of size n+1 */
        seq_t *newdp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!newdp) return 0;          /* out of memory – unlikely in tests */

        /* Initialize to zero */
        for (idx_t i = 0; i <= n; ++i)
            newdp[i] = (seq_t)0;
        newdp[0] = (seq_t)1;

        /* Dynamic programming */
        for (idx_t part = 1; part <= n; ++part) {
            idx_t r = part % 19;
            if (r == 0 || r == 2 || r == 17)
                continue;                  /* forbidden part */
            for (idx_t sum = part; sum <= n; ++sum)
                newdp[sum] = newdp[sum] + newdp[sum - part];
        }

        /* Replace old table */
        if (dp) free(dp);
        dp = newdp;
        max_n = n;
    }

    return dp[n];
}
