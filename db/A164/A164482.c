/*
 * A164482  Number of binary strings of length n with no substrings equal to
 *          0001, 0110, or 1110.
 * https://oeis.org/A164482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 72, u64 n <= 150,
 *            u128 n <= 308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164482: Number of binary strings of length n with no substrings
 *  0001, 0110, or 1110.
 *
 *  This implementation counts valid strings by dynamic programming over
 *  the last three bits of the prefix.  The state space has 8 states
 *  representing the three most recent bits.  When a new bit is appended,
 *  the 4‑bit pattern formed is checked against the forbidden patterns.
 *  The count for each state is stored in an array of type seq_t.
 *  Complexity: O(n) time, O(1) additional space.  Works for every call
 *  order and with any seq_t size allowed by the build system. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* DP arrays for the current and next step (size 8 for 3‑bit suffixes). */
    seq_t dp_cur[8] = {0};
    seq_t dp_next[8] = {0};

    /* Initially the empty string has suffix 000. */
    dp_cur[0] = (seq_t)1;

    /* Transition from length 0 up to n‑1 to build strings of length n. */
    for (idx_t len = 0; len < n; ++len)
    {
        /* reset next array */
        for (int s = 0; s < 8; ++s) dp_next[s] = (seq_t)0;

        /* iterate over all suffixes of the current prefix */
        for (int s = 0; s < 8; ++s)
        {
            seq_t cnt = dp_cur[s];
            if (cnt == (seq_t)0) continue;

            /* try appending bit 0 and 1 */
            for (int bit = 0; bit < 2; ++bit)
            {
                /* For prefixes of length >=3 a 4‑bit pattern is formed. */
                if (len >= 3)
                {
                    int pat = ((s << 1) | bit) & 0xF;   /* 4 bits */
                    if (pat == 1 || pat == 6 || pat == 14)   /* 0001, 0110, 1110 */
                        continue;   /* forbidden, skip */
                }

                int new_s = ((s << 1) | bit) & 0x7;   /* keep last 3 bits */
                dp_next[new_s] += cnt;
            }
        }

        /* swap dp_cur and dp_next for next iteration */
        for (int s = 0; s < 8; ++s) dp_cur[s] = dp_next[s];
    }

    /* Sum all states to get the total number of valid strings of length n. */
    seq_t total = (seq_t)0;
    for (int s = 0; s < 8; ++s) total += dp_cur[s];

    return total;
}
