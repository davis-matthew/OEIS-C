/*
 * A224808  Number of permutations (p(1), p(2), ..., p(n)) satisfying -k <=
 *          p(i)-i <= r and p(i)-i not in the set I, i=1..n, with k=2, r=6,
 *          I={-1,1,2,3,4,5}.
 * https://oeis.org/A224808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 72, u64 n <= 141,
 *            u128 n <= 279, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of subsets of {1..n-6} with no differences 2, 4 or 6.
   Computed by a simple linear‑time DP over 6‑bit masks. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 6 the set {1..n-6} is empty, so only the empty subset exists. */
    if (n < 6)
        return (seq_t)1;

    /* Dynamic programming over m = n - 6 positions. */
    const idx_t m = n - 6;
    seq_t dp[64] = {0};
    seq_t next[64] = {0};
    dp[0] = (seq_t)1;          /* empty configuration at start */

    for (idx_t pos = 0; pos < m; ++pos) {
        /* Clear next array. */
        for (int mask = 0; mask < 64; ++mask)
            next[mask] = 0;

        /* Transition from all current states. */
        for (int mask = 0; mask < 64; ++mask) {
            seq_t val = dp[mask];
            if (!val)
                continue;

            /* Option 1: leave current position empty. */
            int newmask0 = (mask << 1) & 0x3F;
            next[newmask0] += val;

            /* Option 2: place a 1 at current position, if allowed. */
            /* Forbidden differences are 2, 4, 6 -> mask bits 1, 3, 5 must be 0. */
            if ((mask & 0x2A) == 0) {     /* 0x2A = 00101010b */
                int newmask1 = newmask0 | 1;
                next[newmask1] += val;
            }
        }

        /* Copy next back to dp for the next iteration. */
        for (int mask = 0; mask < 64; ++mask)
            dp[mask] = next[mask];
    }

    /* Sum all states to obtain the total number of valid subsets. */
    seq_t result = 0;
    for (int mask = 0; mask < 64; ++mask)
        result += dp[mask];

    return result;
}
