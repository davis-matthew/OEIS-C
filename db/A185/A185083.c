/*
 * A185083  Partitions of 2*n into parts not congruent to 0, +-2, +-12, +-14,
 *          16 (mod 32).
 * https://oeis.org/A185083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 80, u64 n <= 266,
 *            u128 n <= 946, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partitions of 2*n into parts not congruent to 0, ±2, ±12, ±14, 16 (mod 32)
   The number is computed by dynamic programming:  dp[s] = sum_{part in allowed}
   dp[s - part].  dp[0] = 1.  The table grows only when a larger n is requested. */

#include <stdlib.h>
#include <stdint.h>

/* Check whether a part size is forbidden modulo 32.
   Forbidden residues are 0, 2, 12, 14, 16, 18, 20, 30. */
static inline int is_forbidden(idx_t v)
{
    static const unsigned char forb[32] =
    { /* 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 */
      1, 0, 1, 0, 0, 0, 0, 0, /*  0-7 */
      0, 0, 0, 0, 1, 0, 1, 0, /*  8-15 */
      1, 0, 1, 0, 1, 0, 0, 0, /* 16-23 */
      0, 0, 0, 0, 0, 0, 1, 0  /* 24-31 */
    };
    return forb[(int)(v & 31ULL)];
}

seq_t a(idx_t n)
{
    /* a(0) = 1 without allocating the DP array. */
    if (n == 0)
        return (seq_t)1;

    /* Static DP table that remembers results up to the largest n requested. */
    static idx_t max_n = 0;       /* largest n for which dp is complete   */
    static seq_t *dp = NULL;      /* dp[0 .. 2*max_n]                      */
    static idx_t max_s = 0;       /* size‑1 of dp (i.e. 2*max_n)           */

    if (n > max_n) {
        /* Rebuild the DP table up to the new n.  Old table is freed. */
        if (dp != NULL)
            free(dp);

        max_n = n;
        max_s = 2ULL * max_n;

        dp = (seq_t *)malloc((size_t)(max_s + 1) * sizeof(seq_t));
        if (!dp)
            return (seq_t)0;   /* allocation failure – should not occur */

        /* Initialise: dp[0] = 1, others 0 */
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i <= max_s; ++i)
            dp[i] = (seq_t)0;

        /* Compute partitions for all s up to max_s using allowed parts. */
        for (idx_t coin = 1; coin <= max_s; ++coin) {
            if (is_forbidden(coin))
                continue;          /* skip forbidden residue classes */

            for (idx_t s = coin; s <= max_s; ++s) {
                dp[s] += dp[s - coin];
            }
        }
    }

    return dp[2ULL * n];
}
