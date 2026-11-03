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
 * Validated: standard, pass, 42 terms through n = 181 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n X 4 binary arrays with no element equal to the sum modulo 3
   of its horizontal and vertical neighbors. */
#include <stdbool.h>
#include <stdint.h>

static void init_tables(bool horiz_valid[16],
                        bool pair_up_valid[16][16],
                        bool pair_down_valid[16][16],
                        bool triple_valid[16][16][16])
{
    /* horizontal validity for a single row */
    for (int state = 0; state < 16; ++state) {
        bool ok = true;
        for (int j = 0; j < 4 && ok; ++j) {
            int left  = (j > 0)     ? ((state >> (j-1)) & 1) : 0;
            int right = (j < 3)     ? ((state >> (j+1)) & 1) : 0;
            int horiz_sum = left + right;
            int val = (state >> j) & 1;
            if (val == (horiz_sum % 3)) ok = false;
        }
        horiz_valid[state] = ok;
    }

    /* pair validity when prev is the top row (no upper neighbor) */
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                int left  = (j > 0)     ? ((prev >> (j-1)) & 1) : 0;
                int right = (j < 3)     ? ((prev >> (j+1)) & 1) : 0;
                int horiz_sum = left + right;
                int down = (curr >> j) & 1;
                int val = (prev >> j) & 1;
                if (val == ((horiz_sum + down) % 3)) ok = false;
            }
            pair_up_valid[prev][curr] = ok;

            ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                int left  = (j > 0)     ? ((curr >> (j-1)) & 1) : 0;
                int right = (j < 3)     ? ((curr >> (j+1)) & 1) : 0;
                int horiz_sum = left + right;
                int up  = (prev >> j) & 1;
                int val = (curr >> j) & 1;
                if (val == ((horiz_sum + up) % 3)) ok = false;
            }
            pair_down_valid[prev][curr] = ok;
        }
    }

    /* triple validity for middle row */
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            for (int next = 0; next < 16; ++next) {
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    int left  = (j > 0)     ? ((curr >> (j-1)) & 1) : 0;
                    int right = (j < 3)     ? ((curr >> (j+1)) & 1) : 0;
                    int horiz_sum = left + right;
                    int up  = (prev >> j) & 1;
                    int down = (next >> j) & 1;
                    int val = (curr >> j) & 1;
                    if (val == ((horiz_sum + up + down) % 3)) ok = false;
                }
                triple_valid[prev][curr][next] = ok;
            }
        }
    }
}

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
