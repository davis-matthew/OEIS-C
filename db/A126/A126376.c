/*
 * A126376  Number of base 22 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1. For n≥1, the number of length‑n base‑22
   strings with adjacent digits differing by at most one is
   computed by dynamic programming.  Let dp[d] be the count of
   length‑k strings ending in digit d (0≤d<22).  Initialize dp[d]=1
   for k=1.  Update for each additional position:     next[d] =
   dp[d] + (d>0?dp[d-1]:0) + (d<21?dp[d+1]:0).  After processing n
   positions, a(n)=∑₀¹¹⁺¹dp[d].                            */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t BASE = 22;
    if (n == 0)
        return 1;                   /* a(0) = 1 */

    seq_t dp[BASE], next[BASE];

    /* k = 1: one‑digit strings are all 22 digits */
    for (idx_t d = 0; d < BASE; ++d)
        dp[d] = 1;

    /* build up to length n */
    for (idx_t len = 2; len <= n; ++len) {
        for (idx_t d = 0; d < BASE; ++d) {
            seq_t val = dp[d];
            if (d > 0)       val += dp[d - 1];
            if (d + 1 < BASE) val += dp[d + 1];
            next[d] = val;
        }
        /* copy next to dp for next iteration */
        for (idx_t d = 0; d < BASE; ++d)
            dp[d] = next[d];
    }

    /* sum all endings to get total strings of length n */
    seq_t total = 0;
    for (idx_t d = 0; d < BASE; ++d)
        total += dp[d];

    return total;
}
