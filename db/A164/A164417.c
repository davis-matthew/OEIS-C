/*
 * A164417  Number of binary strings of length n with no substrings equal to
 *          0000, 0010, or 0100.
 * https://oeis.org/A164417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Uses a 3‑bit DFA forbidding substrings 0000, 0010, 0100. a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=8; for n≥4 the values are obtained from the
 *            DFA transitions
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 77, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 8‑state DP: state bits represent the last up to 3 bits
       (padded with leading zeros for strings shorter than 3). */

    static seq_t state[8] = {0};
    static seq_t next[8]  = {0};
    static int    initialized = 0;
    static idx_t cur_len = 0;          /* length of strings represented by 'state' */

    if (!initialized) {
        for (int i = 0; i < 8; ++i) state[i] = (seq_t)0;
        state[0] = (seq_t)1;          /* empty string, suffix 000 */
        initialized = 1;
    }

    /* Grow DP up to the requested length n */
    for (idx_t i = cur_len + 1; i <= n; ++i) {
        /* Reset next */
        for (int j = 0; j < 8; ++j) next[j] = (seq_t)0;

        int new_len = (int)i;          /* new string length after appending one bit */

        for (int s = 0; s < 8; ++s) {
            seq_t cnt = state[s];
            if (cnt == (seq_t)0) continue;

            for (int b = 0; b <= 1; ++b) {
                if (new_len <= 3) {
                    /* No 4‑bit substring yet; all transitions allowed */
                    int ns = ((s << 1) | b) & 7;
                    next[ns] += cnt;
                } else {
                    /* Full 4‑bit substring exists; forbid 0000(0), 0010(2), 0100(4) */
                    int pattern = (s << 1) | b;      /* 4‑bit value (0..15) */
                    if (pattern == 0 || pattern == 2 || pattern == 4) continue;
                    int ns = pattern & 7;           /* keep last 3 bits of new suffix */
                    next[ns] += cnt;
                }
            }
        }

        /* Move to next state */
        for (int j = 0; j < 8; ++j) state[j] = next[j];

        cur_len = (idx_t)new_len;
    }

    /* Sum all counts to get total number of admissible strings of length n */
    seq_t total = (seq_t)0;
    for (int j = 0; j < 8; ++j) total += state[j];

    return total;
}
