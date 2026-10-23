/*
 * A035974  Number of partitions of n into parts not of the form 19k, 19k+5 or
 *          19k-5. Also number of partitions with at most 4 parts of size 1
 *          and differences between parts at distance 8 are greater than 1.
 * https://oeis.org/A035974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035974
   Number of partitions of n into parts not congruent to 0, 5 or 14 (mod 19).
   Computation is done by a standard coin‑change DP:  dp[s] = number of
   partitions of s using allowed parts up to s.  The routine builds the
   table up to the requested n for each call; this is O(n²) time and
   satisfies the required performance for n ≤ 10 000. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for 0, matching the offset */

    /* Static DP cache reused between calls.  It is large enough to hold
       the table up to the largest n requested so far. */
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    if (n > max_n) {
        /* Grow the array if necessary.  Use realloc to preserve previous
           contents; we will overwrite them anyway. */
        dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;   /* allocation failure */
        max_n = n;
    }

    /* Initialise dp[0] = 1 (empty partition) and all other entries to 0. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Outer loop over all allowed part sizes p.  A part is forbidden if
       p % 19 == 0, 5 or 14. */
    for (idx_t p = 1; p <= n; ++p) {
        idx_t r = p % 19;
        if (r == 0 || r == 5 || r == 14) continue;   /* forbidden part */

        for (idx_t s = p; s <= n; ++s) {
            dp[s] += dp[s - p];
        }
    }

    return dp[n];
}
