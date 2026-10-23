/*
 * A183366  Number of n X 4 binary arrays with no element equal to the sum
 *          modulo 3 of its horizontal and vertical neighbors.
 * https://oeis.org/A183366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   init_tables
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n X 4 binary arrays with no element equal to the sum modulo 3
   of its horizontal and vertical neighbors. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/init_tables.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static bool horiz_valid[16];
    static bool pair_up_valid[16][16];
    static bool pair_down_valid[16][16];
    static bool triple_valid[16][16][16];

    if (!initialized) {
        init_tables(horiz_valid, pair_up_valid, pair_down_valid, triple_valid);
        initialized = true;
    }

    if (n == 1) {
        seq_t sum = 0;
        for (int s = 0; s < 16; ++s)
            if (horiz_valid[s])
                sum += (seq_t)1;
        return sum;
    }

    seq_t dp[16][16] = {0};
    seq_t newdp[16][16] = {0};

    /* initial two rows */
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (pair_up_valid[prev][curr])
                dp[prev][curr] = (seq_t)1;
        }
    }

    /* process rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* reset newdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = (seq_t)0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int next = 0; next < 16; ++next) {
                    if (triple_valid[prev][curr][next])
                        newdp[curr][next] += ways;
                }
            }
        }

        /* copy newdp into dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* final row validation */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (pair_down_valid[prev][curr])
                ans += dp[prev][curr];
        }
    }
    return ans;
}
