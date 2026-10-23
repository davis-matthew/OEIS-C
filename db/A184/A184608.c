/*
 * A184608  Half the number of (n+1)X4 binary arrays with no 2X2 subblock sum
 *          differing from a horizontal or vertical neighbor subblock sum by
 *          more than one.
 * https://oeis.org/A184608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * number of (n+1)×4 binary arrays satisfying the
 *            adjacent 2×2 subblock sum difference constraint. Computed via
 *            dynamic programming on 4‑bit row patterns (states 16×16)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static data computed once */
    static bool good_pair[16][16];
    static uint8_t sum[16][16][3];
    static uint8_t next_row[256][16];
    static uint8_t next_len[256];
    static bool initialized = false;

    if (!initialized) {
        /* Precompute pair properties */
        for (unsigned a = 0; a < 16; ++a) {
            for (unsigned b = 0; b < 16; ++b) {
                uint8_t s[3];
                for (unsigned j = 0; j < 3; ++j) {
                    uint8_t bits = ((a >> j) & 1) + ((a >> (j + 1)) & 1)
                                 + ((b >> j) & 1) + ((b >> (j + 1)) & 1);
                    s[j] = bits;
                }
                bool horiz = ((s[0] > s[1] ? s[0] - s[1] : s[1] - s[0]) <= 1) &&
                             ((s[1] > s[2] ? s[1] - s[2] : s[2] - s[1]) <= 1);
                good_pair[a][b] = horiz;
                sum[a][b][0] = s[0];
                sum[a][b][1] = s[1];
                sum[a][b][2] = s[2];
            }
        }

        /* Precompute transitions between consecutive pairs */
        for (unsigned a = 0; a < 16; ++a) {
            for (unsigned b = 0; b < 16; ++b) {
                unsigned src = (a << 4) | b;
                next_len[src] = 0;
                if (!good_pair[a][b]) continue;
                for (unsigned c = 0; c < 16; ++c) {
                    if (!good_pair[b][c]) continue;
                    bool compat = true;
                    for (unsigned j = 0; j < 3; ++j) {
                        unsigned diff = sum[a][b][j] > sum[b][c][j]
                                        ? sum[a][b][j] - sum[b][c][j]
                                        : sum[b][c][j] - sum[a][b][j];
                        if (diff > 1) { compat = false; break; }
                    }
                    if (compat) {
                        next_row[src][next_len[src]++] = (uint8_t)c;
                    }
                }
            }
        }
        initialized = true;
    }

    /* DP over rows: dp[a][b] = number of ways to end with rows (a,b) */
    seq_t dp_cur[256] = {0};
    seq_t dp_next[256] = {0};

    /* Initial pairs (first two rows) */
    for (unsigned a = 0; a < 16; ++a) {
        for (unsigned b = 0; b < 16; ++b) {
            if (good_pair[a][b]) {
                unsigned idx = (a << 4) | b;
                dp_cur[idx] = 1;
            }
        }
    }

    /* Add additional rows (total rows = n+1, already have 2 rows) */
    idx_t iter = n > 1 ? n - 1 : 0; /* number of extra rows to add */
    for (idx_t step = 0; step < iter; ++step) {
        /* zero next DP array */
        for (unsigned i = 0; i < 256; ++i) dp_next[i] = 0;

        /* transition */
        for (unsigned a = 0; a < 16; ++a) {
            for (unsigned b = 0; b < 16; ++b) {
                unsigned src = (a << 4) | b;
                seq_t val = dp_cur[src];
                if (val == 0) continue;
                unsigned len = next_len[src];
                for (unsigned t = 0; t < len; ++t) {
                    unsigned c = next_row[src][t];
                    unsigned dst = (b << 4) | c;
                    dp_next[dst] += val;
                }
            }
        }
        /* copy dp_next to dp_cur for next iteration */
        for (unsigned i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all DP states to get total number of arrays */
    seq_t total = 0;
    for (unsigned i = 0; i < 256; ++i) total += dp_cur[i];

    return total / 2; /* half of the total number of arrays */
}
