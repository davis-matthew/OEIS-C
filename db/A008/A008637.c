/*
 * A008637  Number of partitions of n into at most 8 parts.
 * https://oeis.org/A008637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts <= 8 (equivalently
 *            partitions of n into at most 8 parts). This uses the standard
 *            coin‑change DP: a(0) = 1 for each part k = 1 .. 8 for n = k .. N
 *            a[n] += a[n-k]
 * Range:     u8 n <= 17, u16 n <= 57, u32 n <= 347, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required external types:
   seq_t : unsigned integer type (e.g. uint8_t ... __int128 or a bignum class)
   idx_t : unsigned integer type (uint64_t)               */

/* Note: The function keeps a static cache so that repeated calls
   with the same or larger n are efficient. */
#include <stdlib.h>

/* static cache of computed values */
static seq_t *dp = NULL;   /* array of a(0)...a(cap-1) */
static idx_t cap = 0;      /* number of elements stored (= current maximum n + 1) */

seq_t a(idx_t n)
{
    /* grow the cache if necessary */
    if (n >= cap) {
        /* new capacity is n+1 elements */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!new_dp) abort();       /* allocation failure */
        dp = new_dp;
        cap = n + 1;

        /* initialise: a(0)=1, others 0 */
        dp[0] = 1;
        for (idx_t i = 1; i < cap; ++i) dp[i] = 0;

        /* dynamic programming over part sizes 1..8 */
        for (idx_t part = 1; part <= 8; ++part) {
            for (idx_t i = part; i < cap; ++i) {
                dp[i] += dp[i - part];
            }
        }
    }

    return dp[n];
}
