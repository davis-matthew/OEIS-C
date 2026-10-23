/*
 * A164498  Number of binary strings of length n with no substrings equal to
 *          0010 0110 or 1100.
 * https://oeis.org/A164498
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the number of binary strings of length n that
   avoid the forbidden 4‑bit substrings 0010, 0110 and 1100.
   Implementation uses a simple DP over the suffix of the last three
   bits.  For each length i (0 ≤ i < n) and each suffix s (0 ≤ s < 8)
   we try to append a 0 or a 1, skipping any append that would create
   one of the forbidden patterns.  The DP runs in O(n) time and uses
   only 8 states.  It works for any n ≥ 4 and remains fast for n ≈ 10000. */

#include <stddef.h>   /* for size_t (used by memset, if desired) */

seq_t a(idx_t n)
{
    /* State arrays: counts for each possible suffix of the last
       three bits.  We use two arrays and alternate between them. */
    seq_t cur[8], nxt[8];
    int s, b;

    /* Initialize: empty string has suffix 0. */
    for (s = 0; s < 8; ++s) cur[s] = 0;
    cur[0] = 1;

    for (idx_t i = 0; i < n; ++i) {
        /* zero the next array */
        for (s = 0; s < 8; ++s) nxt[s] = 0;

        for (s = 0; s < 8; ++s) {
            seq_t val = cur[s];
            if (!val) continue;

            for (b = 0; b < 2; ++b) {
                /* If we already have at least 3 bits, check the new
                   4‑bit pattern for a forbidden substring. */
                if (i >= 3) {
                    int pat = (s << 1) | b;      /* 4‑bit pattern (0–15) */
                    if (pat == 0x2 || pat == 0x6 || pat == 0xC)
                        continue;  /* forbidden pattern detected */
                }

                int ns = ((s << 1) | b) & 7;  /* new suffix (last 3 bits) */
                nxt[ns] = nxt[ns] + val;      /* accumulate count */
            }
        }

        /* swap cur and nxt for the next iteration */
        for (s = 0; s < 8; ++s) cur[s] = nxt[s];
    }

    /* Sum all states to obtain the total number of valid strings. */
    seq_t total = 0;
    for (s = 0; s < 8; ++s) total = total + cur[s];
    return total;
}
