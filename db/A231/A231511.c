/*
 * A231511  Number of nX4 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors.
 * https://oeis.org/A231511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with no element less than a
 *            strict majority of its horizontal, vertical, diagonal and
 *            antidiagonal neighbors. The function uses a transfer‑matrix
 *            dynamic programming over consecutive row pairs
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */

/* Pre‑computed bits for all 4‑bit patterns. */
static const unsigned char bits[16][4] = {
    {0,0,0,0},
    {0,0,0,1},
    {0,0,1,0},
    {0,0,1,1},
    {0,1,0,0},
    {0,1,0,1},
    {0,1,1,0},
    {0,1,1,1},
    {1,0,0,0},
    {1,0,0,1},
    {1,0,1,0},
    {1,0,1,1},
    {1,1,0,0},
    {1,1,0,1},
    {1,1,1,0},
    {1,1,1,1}
};

/* Check whether a given 4‑bit row 'cur' satisfies the strict‑majority rule
   with optional above and below rows. */
static bool valid_row(int cur, bool aboveExists, int above, bool belowExists, int below)
{
    const unsigned char *b_cur = bits[cur];
    for (int c = 0; c < 4; ++c) {
        int sum = 0, v = 0;
        /* horizontal neighbors */
        if (c > 0) { sum += b_cur[c-1]; v++; }
        if (c < 3) { sum += b_cur[c+1]; v++; }
        /* upper neighbors if present */
        if (aboveExists) {
            const unsigned char *b_ab = bits[above];
            if (c > 0) { sum += b_ab[c-1]; v++; }
            sum += b_ab[c]; v++;
            if (c < 3) { sum += b_ab[c+1]; v++; }
        }
        /* lower neighbors if present */
        if (belowExists) {
            const unsigned char *b_bl = bits[below];
            if (c > 0) { sum += b_bl[c-1]; v++; }
            sum += b_bl[c]; v++;
            if (c < 3) { sum += b_bl[c+1]; v++; }
        }
        /* A 0 is forbidden if the majority of neighbors are 1. */
        if (sum > v/2 && b_cur[c] == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the offset 1 sequence */
    if (n == 1) {
        seq_t total = 0;
        for (int cur = 0; cur < 16; ++cur) {
            if (valid_row(cur, false, 0, false, 0))
                total += (seq_t)1;
        }
        return total;
    }

    seq_t dp[256];
    seq_t next[256];
    for (int i = 0; i < 256; ++i) { dp[i] = 0; next[i] = 0; }

    /* Count of first row patterns (each counted once). */
    seq_t first[16];
    for (int i = 0; i < 16; ++i) first[i] = (seq_t)1;

    /* Process the second row. */
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            if (valid_row(a, false, 0, true, b))
                dp[(a << 4) | b] += first[a];
        }
    }

    /* Process rows 3 .. n. */
    for (idx_t r = 2; r < n; ++r) {
        for (int i = 0; i < 256; ++i) next[i] = 0;
        for (int state = 0; state < 256; ++state) {
            seq_t cnt = dp[state];
            if (cnt == 0) continue;
            int prev = state >> 4;
            int cur  = state & 0xF;
            for (int nb = 0; nb < 16; ++nb) {
                if (valid_row(cur, true, prev, true, nb))
                    next[(cur << 4) | nb] += cnt;
            }
        }
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* Final check for the bottom row. */
    seq_t total = 0;
    for (int state = 0; state < 256; ++state) {
        seq_t cnt = dp[state];
        if (cnt == 0) continue;
        int prev = state >> 4;
        int cur  = state & 0xF;
        if (valid_row(cur, true, prev, false, 0))
            total += cnt;
    }
    return total;
}
