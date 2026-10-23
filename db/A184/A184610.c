/*
 * A184610  Half the number of (n+1)X6 binary arrays with no 2X2 subblock sum
 *          differing from a horizontal or vertical neighbor subblock sum by
 *          more than one.
 * https://oeis.org/A184610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/2 * (# of (n+1)×6 binary arrays with no 2×2 subblock
 *            sum differing from a horizontal or vertical neighbor subblock
 *            sum by more than one)
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Acceptable types from the build environment are provided in the compilation
   environment as seq_t (unsigned integer or arbitrary‑precision) and idx_t
   (uint64_t). No local definitions are added here. */

static bool horiz_ok[64][64];
static bool vert_ok[64][64];
static uint64_t allow_mask[64][64];
static bool precomp_done = false;

static void precompute(void) {
    /* Horizontal constraints for two rows (r1, r2) */
    for (int r1 = 0; r1 < 64; ++r1) {
        for (int r2 = 0; r2 < 64; ++r2) {
            bool ok = true;
            for (int c = 0; c <= 3; ++c) {
                int s1 = ((r1 >> c) & 1) + ((r1 >> (c + 1)) & 1)
                       + ((r2 >> c) & 1) + ((r2 >> (c + 1)) & 1);
                int s2 = ((r1 >> (c + 1)) & 1) + ((r1 >> (c + 2)) & 1)
                       + ((r2 >> (c + 1)) & 1) + ((r2 >> (c + 2)) & 1);
                int diff = s2 - s1;
                if (diff < 0) diff = -diff;
                if (diff > 1) { ok = false; break; }
            }
            horiz_ok[r1][r2] = ok;
        }
    }

    /* Vertical constraints between row r and row r+2 (separated by one row) */
    for (int r1 = 0; r1 < 64; ++r1) {
        for (int r3 = 0; r3 < 64; ++r3) {
            bool ok = true;
            for (int c = 0; c <= 4; ++c) {
                int s1 = ((r1 >> c) & 1) + ((r1 >> (c + 1)) & 1);
                int s3 = ((r3 >> c) & 1) + ((r3 >> (c + 1)) & 1);
                int diff = s3 - s1;
                if (diff < 0) diff = -diff;
                if (diff > 1) { ok = false; break; }
            }
            vert_ok[r1][r3] = ok;
        }
    }

    /* Allowed transition mask: for states (prev, prev2) -> cur */
    for (int prev = 0; prev < 64; ++prev) {
        for (int prev2 = 0; prev2 < 64; ++prev2) {
            uint64_t m = 0;
            for (int cur = 0; cur < 64; ++cur) {
                if (horiz_ok[prev][cur] && vert_ok[prev2][cur])
                    m |= (uint64_t)1 << cur;
            }
            allow_mask[prev][prev2] = m;
        }
    }

    precomp_done = true;
}

seq_t a(idx_t n) {
    if (!precomp_done) precompute();

    static seq_t dp[64][64];
    static seq_t ndp[64][64];
    /* Initialize dp for rows 0 and 1 */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = (seq_t)0;
    for (int r0 = 0; r0 < 64; ++r0) {
        for (int r1 = 0; r1 < 64; ++r1) {
            if (horiz_ok[r0][r1])
                dp[r1][r0] = (seq_t)1;
        }
    }

    /* DP over additional rows: 2 .. n (array has n+1 rows) */
    for (idx_t step = 2; step <= n; ++step) {
        /* zero ndp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                ndp[i][j] = (seq_t)0;
        for (int prev = 0; prev < 64; ++prev) {
            for (int prev2 = 0; prev2 < 64; ++prev2) {
                seq_t cnt = dp[prev][prev2];
                if (cnt == (seq_t)0) continue;
                uint64_t m = allow_mask[prev][prev2];
                while (m) {
                    int cur = __builtin_ctzll(m);
                    m &= m - 1;
                    ndp[cur][prev] += cnt;
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum all DP states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            total += dp[i][j];

    /* the required term is half the total */
    return total / (seq_t)2;
}
