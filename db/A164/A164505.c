/*
 * A164505  Number of binary strings of length n with no substrings equal to
 *          0011 0101 or 1100.
 * https://oeis.org/A164505
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n with no substring
 *            0011, 0101, or 1100. Computation method: Use a 3‑bit state
 *            automaton (states 0..7 represent the last 3 bits). For each
 *            state and each possible appended bit, check whether the 4‑bit
 *            window matches a forbidden pattern. If not, transition...
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 51, u64 n <= 108,
 *            u128 n <= 222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) {
        /* For n < 4 no forbidden pattern can appear, but the sequence
           is not defined in OEIS.  Returning 2^n is harmless. */
        return (seq_t)1 << (unsigned)n;
    }

    /* dp[s] = number of strings of current length whose last 3 bits
       equal state s (0..7). */
    seq_t dp[8];
    seq_t next[8];
    const seq_t one = (seq_t)1;
    const seq_t zero = (seq_t)0;

    /* After 3 bits, every 3‑bit suffix occurs exactly once. */
    for (int i = 0; i < 8; ++i) dp[i] = one;

    for (idx_t len = 3; len < n; ++len) {
        for (int i = 0; i < 8; ++i) next[i] = zero;
        for (int s = 0; s < 8; ++s) {
            seq_t cur = dp[s];
            if (cur == zero) continue;
            for (int b = 0; b < 2; ++b) {
                int pat = (s << 1) | b;          /* 4‑bit window */
                if (pat == 3 || pat == 5 || pat == 12) continue; /* 0011,0101,1100 */
                int ns = pat & 7;                 /* new state (last 3 bits) */
                next[ns] = next[ns] + cur;
            }
        }
        for (int i = 0; i < 8; ++i) dp[i] = next[i];
    }

    /* Sum all states for the final count. */
    seq_t total = zero;
    for (int i = 0; i < 8; ++i) total = total + dp[i];
    return total;
}
