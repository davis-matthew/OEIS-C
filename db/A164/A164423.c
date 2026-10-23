/*
 * A164423  Number of binary strings of length n with no substrings equal to
 *          0000 0010 or 1100.
 * https://oeis.org/A164423
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   dynamic programming over a DFA on the forbidden substrings 0000,
 *            0010, 1100. See the transition table below
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 82, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int TRANS[8][2] = {
    /* 0: ""     */ { 1, 5 },   /* 0→1, 1→5 */
    /* 1: "0"    */ { 2, 5 },   /* 0→2, 1→5 */
    /* 2: "00"   */ { 3, 4 },   /* 0→3, 1→4 */
    /* 3: "000"  */ { -1, 4 },  /* 0→dead, 1→4 */
    /* 4: "001"  */ { -1, 6 },  /* 0→dead, 1→6 */
    /* 5: "1"    */ { 1, 6 },   /* 0→1, 1→6 */
    /* 6: "11"   */ { 7, 6 },   /* 0→7, 1→6 */
    /* 7: "110"  */ { -1, 5 }   /* 0→dead, 1→5 */
};

seq_t a(idx_t n)
{
    /* Handle small indices (offset is 4, but calculation works for all n). */
    seq_t cur[8] = {0}, nxt[8] = {0};
    cur[0] = 1;                     /* empty string */
    idx_t i;
    for (i = 1; i <= n; ++i) {
        /* zero next state */
        nxt[0] = nxt[1] = nxt[2] = nxt[3] = nxt[4] = nxt[5] = nxt[6] = nxt[7] = 0;
        int s;
        for (s = 0; s < 8; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            int t0 = TRANS[s][0];
            if (t0 >= 0) nxt[t0] += val;
            int t1 = TRANS[s][1];
            if (t1 >= 0) nxt[t1] += val;
        }
        /* copy nxt to cur */
        cur[0] = nxt[0];
        cur[1] = nxt[1];
        cur[2] = nxt[2];
        cur[3] = nxt[3];
        cur[4] = nxt[4];
        cur[5] = nxt[5];
        cur[6] = nxt[6];
        cur[7] = nxt[7];
    }
    /* sum all non‑terminal states */
    seq_t total = cur[0] + cur[1] + cur[2] + cur[3]
                + cur[4] + cur[5] + cur[6] + cur[7];
    return total;
}
