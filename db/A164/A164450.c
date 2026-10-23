/*
 * A164450  Number of binary strings of length n with no substrings equal to
 *          0001 0010 or 1011.
 * https://oeis.org/A164450
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 50, u64 n <= 104,
 *            u128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence from OEIS A164450
 * a(n) = number of binary strings of length n that contain none of the
 * substrings 0001, 0010 or 1011.
 * The algorithm runs in O(n) time: we keep the number of valid strings
 * ending with each 3‑bit suffix and extend them one bit at a time,
 * discarding any transition that would create a forbidden 4‑bit window. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n < 4 the values are irrelevant – the sequence is
     * defined for n ≥ 4. Return 0 in those cases. */
    if (n < 4)
        return (seq_t)0;

    /* dp_cur[s] – number of valid strings of the current length whose
     * last three bits equal the 3‑bit value s (0 – 7). */
    seq_t dp_cur[8], next[8];

    /* Length 3: every 3‑bit string is valid, so each suffix occurs exactly once. */
    for (int s = 0; s < 8; ++s)
        dp_cur[s] = (seq_t)1;

    /* Forbidden 4‑bit patterns represented as 4‑bit integers. */
    const idx_t forbid1 = 0x1;  /* 0001 */
    const idx_t forbid2 = 0x2;  /* 0010 */
    const idx_t forbid3 = 0xB;  /* 1011 */

    /* Extend the strings from length 4 up to the requested length n. */
    for (idx_t len = 4; len <= n; ++len) {
        /* Reset the next‑state counts. */
        for (int s = 0; s < 8; ++s)
            next[s] = (seq_t)0;

        /* For every current 3‑bit suffix, try appending 0 and 1. */
        for (int s = 0; s < 8; ++s) {
            seq_t cnt = dp_cur[s];
            if (cnt == (seq_t)0)
                continue;

            for (int b = 0; b < 2; ++b) {
                idx_t t = ((s << 1) | b) & 0xF;  /* the new 4‑bit window */
                if (t == forbid1 || t == forbid2 || t == forbid3)
                    continue;                   /* forbidden transition */

                int ns = t & 7;                  /* new 3‑bit suffix */
                next[ns] = next[ns] + cnt;
            }
        }

        /* Move to the next length. */
        for (int s = 0; s < 8; ++s)
            dp_cur[s] = next[s];
    }

    /* Sum over all final suffixes to obtain a(n). */
    seq_t total = (seq_t)0;
    for (int s = 0; s < 8; ++s)
        total = total + dp_cur[s];

    return total;
}
