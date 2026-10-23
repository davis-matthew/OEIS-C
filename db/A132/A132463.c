/*
 * A132463  Number of partitions of n into distinct parts congruent to 0 or 1
 *          modulo 3.
 * https://oeis.org/A132463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 122, u32 n <= 364, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A132463: number of partitions of n into distinct parts ≡ 0 or 1 (mod 3).
   Computed by dynamic programming on the generating function
   ∏_{k≥1} (1 + x^{3k}) (1 + x^{3k-2}).  Complexity O(n²) per call. */

#include <stdlib.h>
#include <stdbool.h>

/* If seq_t is an arbitrary‑precision integer, the following code works
   as long as seq_t supports the usual arithmetic operators.  */

seq_t a(idx_t n)
{
    static seq_t *dp   = (seq_t *)NULL;   /* DP array */
    static idx_t  cap  = 0;               /* allocated capacity */

    /* Grow array if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed */
        dp   = tmp;
        cap  = new_cap;
    }

    /* Reset for current n */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;                      /* empty partition */

    /* Dynamic programming */
    for (idx_t v = 1; v <= n; ++v) {
        if (v % 3 == 2)                    /* skip parts ≡ 2 (mod 3) */
            continue;
        for (idx_t s = n; s >= v; --s) {
            dp[s] += dp[s - v];           /* use part v once */
        }
    }

    return dp[n];
}
