/*
 * A164420  Number of binary strings of length n with no substrings equal to
 *          0000, 0010, or 0111.
 * https://oeis.org/A164420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 49, u64 n <= 101,
 *            u128 n <= 204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164420: number of binary strings of length n avoiding substrings
   0000, 0010, or 0111.  Base values:
   a(0)=1, a(1)=2, a(2)=4, a(3)=8.
   For n≥4 a(n) is obtained by a 3‑state DP that tracks the last three bits.
   The recurrence derived from the generating function is a(n)=a(n‑1)-a(n‑6),
   but the DP implementation below directly counts the strings. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the base values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* DP states: the last 3 bits (0..7).  For length 3 each state has count 1. */
    seq_t dp[8];
    for (int i = 0; i < 8; ++i) dp[i] = (seq_t)1;

    /* Process lengths 4 .. n */
    for (idx_t len = 4; len <= n; ++len)
    {
        seq_t ndp[8] = {0};          /* new state counts for current length */
        for (int s = 0; s < 8; ++s)
        {
            seq_t val = dp[s];
            if (val == 0) continue;

            /* append a 0 */
            int new0 = ((s << 1) | 0) & 7;
            int pat0 = ((s << 1) | 0) & 15;
            if (pat0 != 0 && pat0 != 2 && pat0 != 7)
                ndp[new0] += val;

            /* append a 1 */
            int new1 = ((s << 1) | 1) & 7;
            int pat1 = ((s << 1) | 1) & 15;
            if (pat1 != 0 && pat1 != 2 && pat1 != 7)
                ndp[new1] += val;
        }
        /* copy new counts to dp for next iteration */
        for (int i = 0; i < 8; ++i) dp[i] = ndp[i];
    }

    /* sum all ending states to get the total number of valid strings of length n */
    seq_t sum = 0;
    for (int i = 0; i < 8; ++i) sum += dp[i];
    return sum;
}
