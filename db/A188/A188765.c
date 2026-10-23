/*
 * A188765  Number of binary strings of length n with no substrings equal to
 *          00000 or 00100.
 * https://oeis.org/A188765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n with no substring
 *            00000 or 00100. The sequence is computed with a DFA over the
 *            last 4 bits of the string
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 68, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    enum { STATES = 31, MAXLEN = 4 };
    static int initialized = 0;
    static uint8_t len_state[STATES];
    static uint8_t bits_state[STATES];
    static int8_t trans[STATES][2];

    if (!initialized) {
        /* Base table: number of states for lengths < l */
        uint8_t base[5];
        base[0] = 0;
        for (int l = 1; l <= 4; ++l) base[l] = base[l-1] + (1U << (l-1));

        /* Enumerate state ids (len, value) */
        for (int l = 0; l <= 4; ++l) {
            for (int val = 0; val < (1 << l); ++val) {
                int id = base[l] + val;
                len_state[id] = (uint8_t)l;
                bits_state[id] = (uint8_t)val;
            }
        }

        /* Precompute transitions */
        for (int s = 0; s < STATES; ++s) {
            int len = len_state[s];
            int val = bits_state[s];
            for (int b = 0; b <= 1; ++b) {
                if (len == 4) {
                    int last5 = (val << 1) | b;   /* 5‑bit window */
                    if (last5 == 0 || last5 == 4) {   /* 00000 or 00100 */
                        trans[s][b] = -1;
                        continue;
                    }
                }
                int new_val, new_len;
                if (len < 4) {
                    new_val = (val << 1) | b;
                    new_len = len + 1;
                } else {
                    new_val = ((val & 0x7) << 1) | b;   /* drop oldest bit */
                    new_len = 4;
                }
                int nid = base[new_len] + new_val;
                trans[s][b] = (int8_t)nid;
            }
        }
        initialized = 1;
    }

    seq_t cur[STATES];
    seq_t nxt[STATES];
    for (int s = 0; s < STATES; ++s) cur[s] = (seq_t)0;
    cur[0] = (seq_t)1;      /* start with empty string */

    for (idx_t i = 0; i < n; ++i) {
        for (int s = 0; s < STATES; ++s) nxt[s] = (seq_t)0;
        for (int s = 0; s < STATES; ++s) {
            seq_t ways = cur[s];
            if (ways == (seq_t)0) continue;
            for (int b = 0; b <= 1; ++b) {
                int8_t t = trans[s][b];
                if (t < 0) continue;
                nxt[t] += ways;
            }
        }
        for (int s = 0; s < STATES; ++s) cur[s] = nxt[s];
    }

    seq_t total = (seq_t)0;
    for (int s = 0; s < STATES; ++s) total += cur[s];
    return total;
}
