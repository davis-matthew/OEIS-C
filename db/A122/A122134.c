/*
 * A122134  Expansion of Sum_{k>=0} x^(k^2+k)/((1-x)(1-x^2)...(1-x^(2k))).
 * https://oeis.org/A122134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 69, u32 n <= 204, u64 n <= 672,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A122134: number of partitions of n with
   parts congruent to 2–7 or 13–18 modulo 20.  The function
   implements a classic coin‑change DP that rebuilds the
   table from scratch whenever a larger n is requested. */

#include <stdbool.h>
#include <stdlib.h>

/* True iff the part size p is allowed (2‑7 or 13‑18 mod 20). */
static inline bool allowed_part(idx_t p)
{
    idx_t r = p % 20;
    return (r >= 2 && r <= 7) || (r >= 13 && r <= 18);
}

/* Cached DP table: dp[i] = a(i).  dp_size = current capacity. */
static seq_t *dp      = NULL;
static idx_t  dp_size = 0;

/* Compute a(n).  Rebuilds the DP table if n is larger than the
   current cached size. */
seq_t a(idx_t n)
{
    /* First call: allocate table and initialise a(0). */
    if (!dp || n + 1 > dp_size) {
        idx_t new_size = (n + 1 > dp_size) ? n + 1 : dp_size ?: 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return 0 as a safe sentinel. */
            return (seq_t)0;
        }
        dp      = tmp;
        dp_size = new_size;
    }

    /* Reset entries up to n. */
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Classic coin‑change DP over allowed part sizes. */
    for (idx_t p = 2; p <= n; ++p) {
        if (!allowed_part(p))
            continue;
        for (idx_t j = p; j <= n; ++j)
            dp[j] = dp[j] + dp[j - p];
    }

    return dp[n];
}
