/*
 * A183486  Number of n X 3 0..3 arrays with every element equal to either the
 *          sum mod 4 of its vertical neighbors or the sum mod 4 of its
 *          horizontal neighbors.
 * https://oeis.org/A183486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n × 3 arrays over {0,1,2,3} where each element
 *            equals either the sum mod 4 of its vertical neighbors (existing
 *            ones only) or the sum mod 4 of its horizontal neighbors. The
 *            entry is a linear recurrence that is computed by a state‑space
 *            DP of size 64×64. This implementation memoise...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   a_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a_one.h"

static inline int popcnt(uint64_t x) { return (int)__builtin_popcountll(x); }

/* Transition with all neighbors present */
static inline int row_ok(int prev_row, int cur_row, int next_row,
                         int prev_missing, int next_missing,
                         unsigned char row_vals[64][3],
                         unsigned char hsum[64][3]) {
    for (int k = 0; k < 3; ++k) {
        unsigned char curv = row_vals[cur_row][k];
        int horiz_ok = (curv == hsum[cur_row][k]);

        int vsum = 0;
        if (!prev_missing) vsum += row_vals[prev_row][k];
        if (!next_missing) vsum += row_vals[next_row][k];
        vsum &= 3;
        int vert_ok = (curv == vsum);

        if (!horiz_ok && !vert_ok) return 0;
    }
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Static state that survives across calls */
    static int init = 0;
    static unsigned char row_vals[64][3], hsum[64][3];
    static uint64_t trans[64][64];
    static unsigned char top_ok[64][64], bottom_ok[64][64];
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static seq_t *dp_cur = NULL, *dp_next = NULL;   /* 64×64 matrices */
    static size_t dp_rows = 0;                       /* length of arrays represented by dp_cur */

    if (!init) {
        /* Pre‑compute row values and horizontal sums */
        for (int r = 0; r < 64; ++r) {
            int x = r;
            for (int k = 0; k < 3; ++k) { row_vals[r][k] = x & 3; x >>= 2; }
            for (int k = 0; k < 3; ++k) {
                int ls;
                if (k == 0) ls = row_vals[r][1];
                else if (k == 2) ls = row_vals[r][1];
                else ls = (row_vals[r][0] + row_vals[r][2]) & 3;
                hsum[r][k] = ls;
            }
        }
        /* Transition table */
        for (int p = 0; p < 64; ++p)
            for (int c = 0; c < 64; ++c) {
                uint64_t m = 0;
                for (int n = 0; n < 64; ++n)
                    if (row_ok(p, c, n, 0, 0, row_vals, hsum))
                        m |= (1ULL << n);
                trans[p][c] = m;
            }
        /* Pre‑compute top and bottom validity */
        for (int r1 = 0; r1 < 64; ++r1)
            for (int r2 = 0; r2 < 64; ++r2) {
                top_ok[r1][r2] = row_ok(-1, r1, r2, 1, 0, row_vals, hsum);
                bottom_ok[r1][r2] = row_ok(r1, r2, -1, 0, 1, row_vals, hsum);
            }
        /* Allocate DP buffers */
        dp_cur = (seq_t*)calloc(64 * 64, sizeof(seq_t));
        dp_next = (seq_t*)calloc(64 * 64, sizeof(seq_t));
        dp_rows = 0;
        init = 1;
    }

    /* Cache handling */
    if (n < cache_size) return cache[n];

    /* Initialise cache capacity */
    if (n >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap : 16;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        for (size_t i = cache_cap; i < newcap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = newcap;
    }

    /* Special case n = 1 */
    if (n == 1) {
        seq_t r = a_one();
        cache[1] = r;
        cache_size = 2;
        return r;
    }

    /* Ensure DP has been initialised up to current dp_rows */
    if (dp_rows == 0) {
        /* Build for n = 2 */
        for (int r1 = 0; r1 < 64; ++r1)
            for (int r2 = 0; r2 < 64; ++r2)
                if (top_ok[r1][r2])
                    dp_cur[(r1 << 6) | r2] = 1;
        /* Compute a(2) */
        seq_t r2 = 0;
        for (int r1 = 0; r1 < 64; ++r1)
            for (int r2i = 0; r2i < 64; ++r2i)
                if (dp_cur[(r1 << 6) | r2i] && bottom_ok[r1][r2i])
                    r2 += dp_cur[(r1 << 6) | r2i];
        cache[2] = r2;
        cache_size = 3;
        dp_rows = 2;
    }

    /* Extend DP to requested n */
    while (dp_rows < n) {
        /* Clear dp_next */
        for (size_t i = 0; i < 64 * 64; ++i) dp_next[i] = 0;
        /* Transition */
        for (int p = 0; p < 64; ++p)
            for (int c = 0; c < 64; ++c) {
                seq_t cnt = dp_cur[(p << 6) | c];
                if (!cnt) continue;
                uint64_t mask = trans[p][c];
                while (mask) {
                    int nxt = __builtin_ctzll(mask);
                    dp_next[(c << 6) | nxt] += cnt;
                    mask &= mask - 1;
                }
            }
        /* Swap */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
        ++dp_rows;

        /* If we reached the required n, compute a(n) */
        if (dp_rows == n) {
            seq_t rn = 0;
            for (int p = 0; p < 64; ++p)
                for (int c = 0; c < 64; ++c)
                    if (dp_cur[(p << 6) | c] && bottom_ok[p][c])
                        rn += dp_cur[(p << 6) | c];
            cache[(size_t)n] = rn;
            if (n + 1 > cache_size) cache_size = n + 1;
            return rn;
        }
    }

    /* Should never reach here */
    return cache[(size_t)n];
}
