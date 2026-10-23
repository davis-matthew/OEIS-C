/*
 * A164414  Number of binary strings of length n with no substrings equal to
 *          0000, 0001, or 1010.
 * https://oeis.org/A164414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of binary strings of length n with no substring 0000, 0001, or 1010 */
/* Recurrence is implemented via a 8-state automaton over the last 3 bits. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* trivial cases 0..3: all 2^n strings are allowed */
    if (n <= 3) {
        return (seq_t)1 << n;
    }

    /* DP arrays for current and next state counts */
    seq_t cur[8] = {0}, nxt[8] = {0};
    cur[0] = 1;                /* empty prefix, last 3 bits = 000 */

    for (idx_t i = 0; i < n; ++i) {
        for (int s = 0; s < 8; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            /* try appending 0 or 1 */
            for (int b = 0; b <= 1; ++b) {
                int allowed = 1;
                if (i >= 3) {        /* we now have 4 bits to check */
                    int p = ((s << 1) | b) & 0xF; /* 4-bit pattern */
                    if (p == 0 || p == 1 || p == 10) {
                        allowed = 0; /* forbidden patterns 0000, 0001, 1010 */
                    }
                }
                if (allowed) {
                    int ns = ((s << 1) | b) & 7; /* keep last 3 bits */
                    nxt[ns] += val;
                }
            }
        }
        /* prepare for next iteration */
        for (int s = 0; s < 8; ++s) {
            cur[s] = nxt[s];
            nxt[s] = 0;
        }
    }

    /* sum over all ending states */
    seq_t total = 0;
    for (int s = 0; s < 8; ++s) total += cur[s];
    return total;
}
