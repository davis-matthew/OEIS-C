/*
 * A183370  Number of nX3 binary arrays with no element equal to the mod 3 sum
 *          of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n X 3 binary arrays with no element equal to the mod 3 sum
   of its diagonal and antidiagonal neighbors */
#include <stdbool.h>

static inline bool valid_row(int prev, int curr, int next)
{
    for (int j = 0; j < 3; ++j) {
        int sum = 0;
        /* neighbors from the row above */
        if (j > 0)   sum += (prev >> (j - 1)) & 1;
        if (j < 2)   sum += (prev >> (j + 1)) & 1;
        /* neighbors from the row below */
        if (j > 0)   sum += (next >> (j - 1)) & 1;
        if (j < 2)   sum += (next >> (j + 1)) & 1;
        int mod = sum % 3;
        int val = (curr >> j) & 1;
        if (val == mod) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; value for n<1 is irrelevant */

    seq_t dp[8][8];
    seq_t newdp[8][8];

    /* initialise dp for the first row (above it is all zeros) */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = 0;
    for (int curr = 0; curr < 8; ++curr)
        dp[0][curr] = 1;

    /* build rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                for (int next = 0; next < 8; ++next) {
                    if (valid_row(prev, curr, next)) {
                        newdp[curr][next] += ways;
                    }
                }
            }
        }

        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* enforce the bottom‑row constraints using a dummy zero row */
    seq_t ans = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            if (dp[prev][curr] != 0 && valid_row(prev, curr, 0)) {
                ans += dp[prev][curr];
            }
        }
    }
    return ans;
}
