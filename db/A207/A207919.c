/*
 * A207919  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   init
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 4×n binary arrays avoiding
 *  horizontals 000, 101 and verticals 010, 101.
 *  All terms up to n=10000 are computed by a short DP.
 */

#include <stddef.h>
#include "../utils/init.h"

static unsigned char vertical_allowed[16];
static unsigned char trans[256][16];
static unsigned char trans_cnt[256];
static unsigned char pair_allowed[256];

/* compute allowed columns and transition graph */
static void precompute(void) {
    static int computed = 0;
    if (computed) return;
    computed = 1;

    for (int c = 0; c < 16; ++c) {
        unsigned char b0 = (c >> 0) & 1;
        unsigned char b1 = (c >> 1) & 1;
        unsigned char b2 = (c >> 2) & 1;
        unsigned char b3 = (c >> 3) & 1;
        int bad = 0;
        /* check rows 0-2 */
        if ((b0 == 0 && b1 == 1 && b2 == 0) ||
            (b0 == 1 && b1 == 0 && b2 == 1))
            bad = 1;
        /* check rows 1-3 */
        if ((b1 == 0 && b2 == 1 && b3 == 0) ||
            (b1 == 1 && b2 == 0 && b3 == 1))
            bad = 1;
        vertical_allowed[c] = (unsigned char)!bad;
    }

    for (int s = 0; s < 256; ++s) {
        int c1 = s >> 4;
        int c2 = s & 15;
        pair_allowed[s] = vertical_allowed[c1] & vertical_allowed[c2];
    }

    /* build transitions */
    for (int s = 0; s < 256; ++s) {
        trans_cnt[s] = 0;
        if (!pair_allowed[s]) continue;      /* invalid state */
        int c1 = s >> 4;
        int c2 = s & 15;
        for (int c3 = 0; c3 < 16; ++c3) {
            if (!vertical_allowed[c3]) continue;
            int ok = 1;
            for (int r = 0; r < 4; ++r) {
                unsigned char b1 = (c1 >> r) & 1;
                unsigned char b2 = (c2 >> r) & 1;
                unsigned char b3 = (c3 >> r) & 1;
                if ((b1 == 0 && b2 == 0 && b3 == 0) ||
                    (b1 == 1 && b2 == 0 && b3 == 1)) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                int t = (c2 << 4) | c3;
                if (trans_cnt[s] < 16) {
                    trans[s][trans_cnt[s]] = (unsigned char)t;
                    trans_cnt[s]++;
                }
            }
        }
    }
}

seq_t a(idx_t n) {
    init();          /* ensure helper structs allocated */
    precompute();    /* compute allowed columns and transition graph */

    /* count allowed single columns */
    unsigned char count_allowed = 0;
    for (int c = 0; c < 16; ++c)
        if (vertical_allowed[c]) count_allowed++;

    if (n == 0) return (seq_t)0;         /* out of scope, but safe */
    if (n == 1) return (seq_t)count_allowed;
    if (n == 2) return (seq_t)count_allowed * (seq_t)count_allowed;

    static seq_t dp_curr[256];
    static seq_t dp_next[256];
    for (int i = 0; i < 256; ++i) dp_curr[i] = 0;

    /* initialize for length 2: all allowed pairs contribute 1 */
    for (int s = 0; s < 256; ++s)
        if (pair_allowed[s]) dp_curr[s] = 1;

    for (idx_t len = 3; len <= n; ++len) {
        for (int i = 0; i < 256; ++i) dp_next[i] = 0;
        for (int s = 0; s < 256; ++s) {
            if (!pair_allowed[s]) continue;
            seq_t val = dp_curr[s];
            if (val == 0) continue;
            int cnt = trans_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                int t = trans[s][k];
                dp_next[t] += val;
            }
        }
        /* swap */
        for (int i = 0; i < 256; ++i) dp_curr[i] = dp_next[i];
    }

    /* sum over all states for final length n */
    seq_t total = 0;
    for (int s = 0; s < 256; ++s)
        if (pair_allowed[s]) total += dp_curr[s];
    return total;
}
