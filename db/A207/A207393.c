/*
 * A207393  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A207393:  number of 4×n binary arrays avoiding
   0 0 1 and 0 1 0 horizontally and 0 0 1 and 1 1 0 vertically.
   Computation is performed by a transfer‑matrix dynamic program
   over the 8 admissible column patterns. */

#include <stdint.h>
#include <stdbool.h>

/* --------- helper constants ------------------------------------- */
#define COLS 8          /* number of admissible column masks */
#define MASKS 16        /* all 4‑bit masks */

/* column masks that avoid the vertical patterns 001 and 110 */
static const uint8_t valid_col[COLS] = {
    0x0,  /* 0000 */
    0x4,  /* 0100 */
    0x5,  /* 0101 */
    0x7,  /* 0111 */
    0x8,  /* 1000 */
    0xA,  /* 1010 */
    0xB,  /* 1011 */
    0xF   /* 1111 */
};

/* --------- main function ---------------------------------------- */
seq_t a(idx_t n) {
    /* static state: transition matrix, DP arrays and memoisation */
    static bool init = false;
    static bool trans[COLS][COLS][COLS];
    static seq_t dp[COLS][COLS];      /* counts for current length */
    static seq_t newdp[COLS][COLS];   /* temporary counts for next length */
    static seq_t cache[10001 + 1];    /* store results up to max n requested */
    static idx_t current_len = 0;     /* length up to which cache is valid */

    /* one‑time initialisation */
    if (!init) {
        /* precompute admissible transitions */
        for (int a = 0; a < COLS; ++a)
            for (int b = 0; b < COLS; ++b)
                for (int c = 0; c < COLS; ++c) {
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        int ppp = (valid_col[a] >> r) & 1;
                        int pp  = (valid_col[b] >> r) & 1;
                        int p   = (valid_col[c] >> r) & 1;
                        if (ppp == 0 && pp == 0 && p == 1) { ok = false; break; }
                        if (ppp == 0 && pp == 1 && p == 0) { ok = false; break; }
                    }
                    trans[a][b][c] = ok;
                }

        /* base DP for length = 2 (all pairs of admissible columns) */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                dp[i][j] = (seq_t)1;

        /* memoise first two terms */
        cache[0] = 0;            /* unused */
        cache[1] = (seq_t)8;     /* one admissible column */
        cache[2] = (seq_t)64;    /* all pairs of admissible columns */
        current_len = 2;
        init = true;
    }

    /* return cached value if already computed */
    if (n <= current_len)
        return cache[n];

    /* extend DP stepwise until we reach requested n */
    for (idx_t len = current_len + 1; len <= n; ++len) {
        /* clear newdp */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                newdp[i][j] = (seq_t)0;

        /* transition from length-1 to length */
        for (int a = 0; a < COLS; ++a)
            for (int b = 0; b < COLS; ++b) {
                seq_t cnt_ab = dp[a][b];
                if (cnt_ab == 0) continue;
                for (int c = 0; c < COLS; ++c) {
                    if (trans[a][b][c])
                        newdp[b][c] = newdp[b][c] + cnt_ab;
                }
            }

        /* compute total for this length */
        seq_t total = (seq_t)0;
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                total = total + newdp[i][j];

        /* store in cache */
        cache[len] = total;

        /* prepare for next iteration */
        for (int i = 0; i < COLS; ++i)
            for (int j = 0; j < COLS; ++j)
                dp[i][j] = newdp[i][j];
    }

    current_len = n;
    return cache[n];
}
