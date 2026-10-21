/*
 * A208684  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A208684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with no horizontal 000 or 001
 *            and no vertical 011 or 101
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)100;

    /* Precomputed 4‑bit patterns that avoid horizontal 000/001 */
    static const idx_t patterns[10] = {
        0b1111, 0b1110, 0b1101, 0b1100,
        0b1011, 0b1010, 0b0111, 0b0110,
        0b0101, 0b0100
    };

    /* Compatibility tables computed once */
    static uint8_t bits[10][4];
    static uint8_t compat[10][10][10];
    static int initialized = 0;
    if (!initialized) {
        /* Extract individual bits for each pattern */
        for (idx_t p = 0; p < 10; ++p) {
            for (idx_t c = 0; c < 4; ++c)
                bits[p][c] = (patterns[p] >> (3 - c)) & 1;
        }
        /* Precompute vertical compatibility */
        for (idx_t pp = 0; pp < 10; ++pp) {
            for (idx_t pr = 0; pr < 10; ++pr) {
                for (idx_t nr = 0; nr < 10; ++nr) {
                    uint8_t ok = 1;
                    for (idx_t col = 0; col < 4; ++col) {
                        uint8_t a = bits[pp][col];
                        uint8_t b = bits[pr][col];
                        uint8_t c = bits[nr][col];
                        /* forbid 011 and 101 vertically */
                        if ((a == 0 && b == 1 && c == 1) ||
                            (a == 1 && b == 0 && c == 1)) {
                            ok = 0;
                            break;
                        }
                    }
                    compat[pp][pr][nr] = ok;
                }
            }
        }
        initialized = 1;
    }

    /* DP arrays: dp[pprev][prev], newdp[pprev][prev] */
    static seq_t dp[10][10];
    static seq_t newdp[10][10];
    /* Initialize for 2 rows: each pair is possible */
    for (idx_t i = 0; i < 10; ++i)
        for (idx_t j = 0; j < 10; ++j)
            dp[i][j] = (seq_t)1;

    /* Build rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear newdp */
        for (idx_t i = 0; i < 10; ++i)
            for (idx_t j = 0; j < 10; ++j)
                newdp[i][j] = (seq_t)0;
        /* transition */
        for (idx_t pp = 0; pp < 10; ++pp) {
            for (idx_t pr = 0; pr < 10; ++pr) {
                seq_t cur_val = dp[pp][pr];
                if (!cur_val) continue;
                for (idx_t nr = 0; nr < 10; ++nr) {
                    if (!compat[pp][pr][nr]) continue;
                    newdp[pr][nr] += cur_val;
                }
            }
        }
        /* swap dp and newdp */
        for (idx_t i = 0; i < 10; ++i)
            for (idx_t j = 0; j < 10; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum all states to get total for n rows */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < 10; ++i)
        for (idx_t j = 0; j < 10; ++j)
            total += dp[i][j];

    return total;
}
