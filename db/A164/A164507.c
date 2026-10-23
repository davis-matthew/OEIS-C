/*
 * A164507  Number of binary strings of length n with no substrings equal to
 *          0011, 0110, or 1100.
 * https://oeis.org/A164507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n that avoid * the
 *            substrings 0011, 0110, and 1100. * * The computation uses a DFA
 *            with 8 states representing the last * up to three bits of the
 *            current string. A transition that would * create a forbidden
 *            4‑bit window is discarded. The DP runs for * n st...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* state[s] = number of strings of the current length whose last
     * up to three bits correspond to the value s (0..7). */
    seq_t state[8] = {0};
    seq_t next[8];
    int s, b;

    state[0] = 1;  /* empty string */

    for (idx_t len = 0; len < n; ++len) {
        /* prepare next array */
        for (s = 0; s < 8; ++s) next[s] = 0;

        for (s = 0; s < 8; ++s) {
            seq_t cnt = state[s];
            if (!cnt) continue;

            for (b = 0; b < 2; ++b) {
                /* 4‑bit window formed by the previous 3 bits (s)
                 * and the new bit b.  Only meaningful when len>=3. */
                if (len >= 3) {
                    uint8_t pat = ((s << 1) | b) & 0xF;
                    if (pat == 3 || pat == 6 || pat == 12) /* 0011,0110,1100 */
                        continue;
                }

                uint8_t ns = ((s << 1) | b) & 7;   /* new 3‑bit suffix */
                next[ns] += cnt;
            }
        }

        /* copy next to state for the next iteration */
        for (s = 0; s < 8; ++s) state[s] = next[s];
    }

    /* sum all states to obtain the total number of admissible strings */
    seq_t sum = 0;
    for (s = 0; s < 8; ++s) sum += state[s];
    return sum;
}
