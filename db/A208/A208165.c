/*
 * A208165  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208165:  number of 4×n 0‑1 arrays avoiding
 * horizontal 000 and 101, and vertical 001 and 010.
 * Base values: a(1)=9, a(2)=81.  Uses an O(n) dynamic
 * programme with 4²=16 column states.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- pre‑compute column and transition tables ---------- */
    static bool inited = false;
    static unsigned char good[16];                  /* column can appear    */
    static unsigned short trans[256];               /* transition mask    */
    if (!inited) {
        /* Determine which 4‑bit columns satisfy the vertical rules:
         *  rows 0‑1‑2 and 1‑2‑3 cannot be 001 or 010. */
        for (int s = 0; s < 16; ++s) {
            int a0 = (s >> 0) & 1;
            int a1 = (s >> 1) & 1;
            int a2 = (s >> 2) & 1;
            int a3 = (s >> 3) & 1;
            bool ok = true;
            if ((a0 == 0 && a1 == 0 && a2 == 1) ||
                (a0 == 0 && a1 == 1 && a2 == 0))
                ok = false;
            if ((a1 == 0 && a2 == 0 && a3 == 1) ||
                (a1 == 0 && a2 == 1 && a3 == 0))
                ok = false;
            good[s] = ok;
        }

        /* Pre‑compute allowed next columns for each ordered pair
         * (previous, current).  The horizontal constraints are that
         * in each row the triple of bits cannot be 000 or 101. */
        for (int p = 0; p < 16; ++p) {
            for (int q = 0; q < 16; ++q) {
                unsigned short mask = 0;
                if (good[p] && good[q]) {
                    for (int r = 0; r < 16; ++r) {
                        if (!good[r]) continue;
                        bool ok = true;
                        for (int row = 0; row < 4; ++row) {
                            int b0 = (p >> row) & 1;
                            int b1 = (q >> row) & 1;
                            int b2 = (r >> row) & 1;
                            if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                                (b0 == 1 && b1 == 0 && b2 == 1)) {
                                ok = false;
                                break;
                            }
                        }
                        if (ok) mask |= (1u << r);
                    }
                }
                trans[p * 16 + q] = mask;
            }
        }
        inited = true;
    }

    /* ---------- small N ---------- */
    if (n == 1) {
        seq_t sum = 0;
        for (int s = 0; s < 16; ++s) if (good[s]) sum += 1;
        return sum;
    }
    if (n == 2) {
        seq_t cnt = 0;
        for (int p = 0; p < 16; ++p) if (good[p])
            for (int q = 0; q < 16; ++q) if (good[q])
                cnt += 1;
        return cnt;   /* = 9² = 81 */
    }

    /* ---------- DP for N ≥ 3 ---------- */
    seq_t dp[256]  = {0};
    seq_t next[256] = {0};

    /* initialise with all good pairs (columns 1 and 2) */
    for (int p = 0; p < 16; ++p) if (good[p])
        for (int q = 0; q < 16; ++q) if (good[q])
            dp[p * 16 + q] = 1;

    /* iterate adding columns 3 … n */
    for (idx_t col = 3; col <= n; ++col) {
        /* clear next array */
        for (int i = 0; i < 256; ++i) next[i] = 0;

        /* transition */
        for (int idx = 0; idx < 256; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;
            unsigned short mask = trans[idx];
            if (!mask) continue;
            int p = idx >> 4;
            int q = idx & 0xF;

            for (int r = 0; r < 16; ++r)
                if (mask & (1u << r))
                    next[q * 16 + r] += val;
        }
        /* swap dp and next */
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* sum all states after n columns */
    seq_t ans = 0;
    for (int i = 0; i < 256; ++i) ans += dp[i];
    return ans;
}
