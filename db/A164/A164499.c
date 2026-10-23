/*
 * A164499  Number of binary strings of length n with no substrings equal to
 *          0010 0110 or 1101.
 * https://oeis.org/A164499
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 49, u64 n <= 102,
 *            u128 n <= 206, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164499 – number of binary strings of length n with no substrings
   equal to 0010, 0110 or 1101.
   This implementation uses a simple automaton where the state is the
   last up to 3 bits of the current string.  Each transition corresponds
   to appending a new bit; the transition is discarded if it would create
   one of the forbidden patterns.  DP over this automaton yields the
   desired counts. */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

seq_t a(idx_t n)
{
    /* Number of states: lengths 0..3, 1 + 2 + 4 + 8 = 15.  */
    static int state_len[15];       /* length of suffix in each state (0..3) */
    static int state_bits[15];      /* bits of suffix, left‑aligned to length */
    static int state_index[4][8];   /* mapping from (len,bits) -> state index */
    static int trans[15][2];        /* transition table: -1 if forbidden */
    static bool init = false;

    if (!init) {
        /* Build state list and index mapping */
        int idx = 0;
        for (int len = 0; len <= 3; ++len) {
            int max = 1 << len;           /* number of bit patterns of this length */
            for (int bits = 0; bits < max; ++bits) {
                state_len[idx]  = len;
                state_bits[idx] = bits;
                state_index[len][bits] = idx;
                ++idx;
            }
        }
        /* Forbidden 4‑bit patterns: 0010 (2), 0110 (6), 1101 (13) */
        const int bad4[3] = {2, 6, 13};

        for (int s = 0; s < 15; ++s) {
            for (int b = 0; b <= 1; ++b) {
                int len = state_len[s];
                int bits = state_bits[s];
                if (len == 3) {
                    int pat = ((bits << 1) | b) & 0xF;   /* 4‑bit pattern */
                    bool forbidden = false;
                    for (int k = 0; k < 3; ++k) {
                        if (pat == bad4[k]) { forbidden = true; break; }
                    }
                    if (forbidden) {
                        trans[s][b] = -1;
                        continue;
                    }
                    int newbits = ((bits << 1) | b) & 0x7; /* keep last 3 bits */
                    trans[s][b] = state_index[3][newbits];
                } else {
                    /* len < 3: no 4‑bit pattern possible yet */
                    int newlen = len + 1;
                    int newbits = ((bits << 1) | b);
                    trans[s][b] = state_index[newlen][newbits];
                }
            }
        }
        init = true;
    }

    /* DP arrays: current and next counts for each state */
    seq_t curr[15] = {0};
    seq_t next[15];

    curr[0] = 1;  /* start with empty string, state 0 (len 0, bits 0) */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 15; ++i) next[i] = 0;
        for (int s = 0; s < 15; ++s) {
            if (curr[s] == 0) continue;
            seq_t val = curr[s];
            int ns = trans[s][0];
            if (ns != -1) next[ns] += val;
            ns = trans[s][1];
            if (ns != -1) next[ns] += val;
        }
        for (int i = 0; i < 15; ++i) curr[i] = next[i];
    }

    /* Sum all valid strings of length n */
    seq_t total = 0;
    for (int i = 0; i < 15; ++i) total += curr[i];
    return total;
}
