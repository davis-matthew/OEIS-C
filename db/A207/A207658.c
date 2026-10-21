/*
 * A207658  Number of nX5 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming over the last two rows;
 *            initial values a(1)=16, a(2)=256
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned char allowed_rows[16] = {
    0x04, /* 00100 */
    0x05, /* 00101 */
    0x06, /* 00110 */
    0x09, /* 01001 */
    0x0A, /* 01010 */
    0x0B, /* 01011 */
    0x0C, /* 01100 */
    0x0D, /* 01101 */
    0x12, /* 10010 */
    0x13, /* 10011 */
    0x14, /* 10100 */
    0x15, /* 10101 */
    0x16, /* 10110 */
    0x19, /* 11001 */
    0x1A, /* 11010 */
    0x1B  /* 11011 */
};

/* Precomputed bits for quick access: bits[row][col] */
static const unsigned char bits[16][5] = {
    {0,0,1,0,0}, /* 00100 */
    {0,0,1,0,1},
    {0,0,1,1,0},
    {0,1,0,0,1},
    {0,1,0,1,0},
    {0,1,0,1,1},
    {0,1,1,0,0},
    {0,1,1,0,1},
    {1,0,0,1,0},
    {1,0,0,1,1},
    {1,0,1,0,0},
    {1,0,1,0,1},
    {1,0,1,1,0},
    {1,1,0,0,1},
    {1,1,0,1,0},
    {1,1,0,1,1}
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    if (n == 1) return (seq_t)16;   /* number of allowed rows */
    if (n == 2) return (seq_t)256;  /* 16^2 */

    /* Lazy static initialization of compatibility masks */
    static bool comp_init = false;
    static uint16_t compat[16][16];
    if (!comp_init) {
        for (unsigned int p = 0; p < 16; ++p) {
            for (unsigned int c = 0; c < 16; ++c) {
                uint16_t mask = 0;
                for (unsigned int nx = 0; nx < 16; ++nx) {
                    bool ok = true;
                    for (int col = 0; col < 5; ++col) {
                        if (bits[c][col] == 0 && bits[p][col] != bits[nx][col]) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) mask |= (uint16_t)(1u << nx);
                }
                compat[p][c] = mask;
            }
        }
        comp_init = true;
    }

    /* DP arrays: dp[prev][curr] counts for current length */
    static seq_t dp[16][16];
    static seq_t next_dp[16][16];
    for (unsigned int i = 0; i < 16; ++i)
        for (unsigned int j = 0; j < 16; ++j)
            dp[i][j] = (seq_t)1;   /* base for n = 2: all pairs are allowed */

    /* Iterate from row 3 up to n */
    for (idx_t step = 3; step <= n; ++step) {
        for (unsigned int i = 0; i < 16; ++i)
            for (unsigned int j = 0; j < 16; ++j)
                next_dp[i][j] = 0;   /* reset next_dp */

        for (unsigned int p = 0; p < 16; ++p) {
            for (unsigned int c = 0; c < 16; ++c) {
                seq_t val = dp[p][c];
                if (!val) continue;
                uint16_t mask = compat[p][c];
                if (!mask) continue;
                for (unsigned int nx = 0; nx < 16; ++nx) {
                    if (mask & (uint16_t)(1u << nx)) {
                        next_dp[c][nx] += val;
                    }
                }
            }
        }
        /* copy next_dp to dp for next iteration */
        for (unsigned int i = 0; i < 16; ++i)
            for (unsigned int j = 0; j < 16; ++j)
                dp[i][j] = next_dp[i][j];
    }

    /* Sum over all ending pairs to get total number of arrays */
    seq_t total = 0;
    for (unsigned int i = 0; i < 16; ++i)
        for (unsigned int j = 0; j < 16; ++j)
            total += dp[i][j];

    return total;
}
