/*
 * A109703  Number of partitions of n into parts each equal to 1 mod 7.
 * https://oeis.org/A109703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 251, u32 n <= 781, u64 n <= 2656,
 *            u128 n <= 9615, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sequence A109703: Number of partitions of n into parts ≡ 1 (mod 7).
   Computation by dynamic programming.  The recurrence is

      dp[0] = 1
      for each part p = 1, 8, 15, ... ≤ n
          for i = p .. n
              dp[i] += dp[i-p]                (1)
   where dp[i] holds a(i).  The array is enlarged only when needed
   and rebuilt whenever a value beyond the previously computed range is
   requested.  The routine is memoised and safe for out‑of‑order calls. */

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t  *dp   = NULL;   /* array of DP values          */
    static idx_t   cur   = 0;      /* largest index computed     */
    static idx_t   cap   = 0;      /* currently allocated size   */
    static bool    ready = false;  /* indicates that dp[0] has been set */

    /* Allocate or enlarge the array if necessary. */
    if (n + 1 > cap) {
        seq_t *tmp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure */
        if (dp) free(dp);
        dp  = tmp;
        cap = (idx_t)(n + 1);
    }

    /* Recompute the table if this is the first call or if a larger n
       is requested.  This also guarantees dp[0] is initialised. */
    if (!ready || n > cur) {
        /* zero the relevant part of the array */
        for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;   /* empty partition */

        /* dynamic programming over allowed parts */
        for (idx_t part = 1; part <= n; part += 7) {
            for (idx_t i = part; i <= n; ++i) {
                dp[i] = dp[i] + dp[i - part];
            }
        }

        ready = true;
        cur   = n;
    }

    return dp[n];
}
