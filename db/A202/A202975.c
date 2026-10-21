/*
 * A202975  Number of nX4 0..1 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A202975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0..1 arrays in which every 1 has at least
 *            two 1‑neighbors (horizontally or vertically)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* test whether a 1 in position j of the middle row has at least two 1‑neighbors
   among the existing horizontal neighbors and the up/down cells. */
static inline bool row_ok(unsigned up, unsigned mid, unsigned down)
{
    for (int j = 0; j < 4; ++j) {
        if (mid & (1u << j)) {
            int cnt = 0;
            if (j > 0   && (mid & (1u << (j - 1)))) ++cnt;
            if (j < 3   && (mid & (1u << (j + 1)))) ++cnt;
            if (up   & (1u << j)) ++cnt;
            if (down & (1u << j)) ++cnt;
            if (cnt < 2)
                return false;
        }
    }
    return true;
}

/* validity table: valid[up][mid][down] == true if mid is allowed
   when the row above is 'up' and the row below is 'down'. */
static unsigned char valid[16][16][16];
static bool precomputed = false;
static void init_valid(void)
{
    for (unsigned up = 0; up < 16; ++up)
        for (unsigned mid = 0; mid < 16; ++mid)
            for (unsigned down = 0; down < 16; ++down)
                valid[up][mid][down] = row_ok(up, mid, down);
    precomputed = true;
}

seq_t a(idx_t n)
{
    if (!precomputed) init_valid();

    seq_t dp[16][16];
    seq_t newdp[16][16];

    /* zero all entries */
    for (unsigned u = 0; u < 16; ++u)
        for (unsigned v = 0; v < 16; ++v)
            dp[u][v] = 0;

    /* initial state: imaginary top row is all zeros */
    for (unsigned row = 0; row < 16; ++row)
        dp[0][row] = 1;

    /* iterate over rows 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i) {
        /* zero newdp */
        for (unsigned u = 0; u < 16; ++u)
            for (unsigned v = 0; v < 16; ++v)
                newdp[u][v] = 0;

        for (unsigned up = 0; up < 16; ++up) {
            for (unsigned prev = 0; prev < 16; ++prev) {
                seq_t val = dp[up][prev];
                if (val == 0) continue;
                for (unsigned cur = 0; cur < 16; ++cur) {
                    if (!valid[up][prev][cur]) continue;
                    newdp[prev][cur] += val;
                }
            }
        }
        /* copy newdp into dp for next iteration */
        for (unsigned u = 0; u < 16; ++u)
            for (unsigned v = 0; v < 16; ++v)
                dp[u][v] = newdp[u][v];
    }

    /* final verification of the last row (down == 0) */
    seq_t result = 0;
    for (unsigned up = 0; up < 16; ++up) {
        for (unsigned last = 0; last < 16; ++last) {
            if (!valid[up][last][0]) continue;
            result += dp[up][last];
        }
    }
    return result;
}
