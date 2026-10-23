/*
 * A183371  Number of nX4 binary arrays with no element equal to the mod 3 sum
 *          of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = number of n×4 binary arrays with no element equal to
   the mod 3 sum of its diagonal and anti‑diagonal neighbors.
   Computed by dynamic programming over rows. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) is just the single all‑1 array */
    if (n <= 1)
        return (n == 1) ? 1 : 0;

    /* Compatibility tables, computed once */
    static bool ok_first[16][16];
    static bool ok_last[16][16];
    static bool ok_mid[16][16][16];
    static bool tables_init = false;
    if (!tables_init) {
        tables_init = true;
        for (int r0 = 0; r0 < 16; ++r0) {
            for (int r1 = 0; r1 < 16; ++r1) {
                /* first row, only below neighbor */
                bool good = true;
                for (int j = 0; j < 4; ++j) {
                    int sum = 0;
                    if (j > 0)   sum += (r1 >> (j-1)) & 1;
                    if (j < 3)   sum += (r1 >> (j+1)) & 1;
                    int bit = (r0 >> j) & 1;
                    if (bit == (sum % 3)) { good = false; break; }
                }
                ok_first[r0][r1] = good;

                /* last row, only above neighbor (same test as first) */
                good = true;
                for (int j = 0; j < 4; ++j) {
                    int sum = 0;
                    if (j > 0)   sum += (r1 >> (j-1)) & 1;
                    if (j < 3)   sum += (r1 >> (j+1)) & 1;
                    int bit = (r0 >> j) & 1;
                    if (bit == (sum % 3)) { good = false; break; }
                }
                ok_last[r0][r1] = good;
            }
        }

        /* interior rows: need both neighbours */
        for (int prev = 0; prev < 16; ++prev)
            for (int mid = 0; mid < 16; ++mid)
                for (int next = 0; next < 16; ++next) {
                    bool good = true;
                    for (int j = 0; j < 4; ++j) {
                        int sum = 0;
                        if (j > 0)   sum += (prev >> (j-1)) & 1;
                        if (j < 3)   sum += (prev >> (j+1)) & 1;
                        if (j > 0)   sum += (next >> (j-1)) & 1;
                        if (j < 3)   sum += (next >> (j+1)) & 1;
                        int bit = (mid >> j) & 1;
                        if (bit == (sum % 3)) { good = false; break; }
                    }
                    ok_mid[prev][mid][next] = good;
                }
    }

    /* Dynamic programming over rows */
    seq_t dp[16][16] = {0};
    seq_t next[16][16] = {0};

    /* initialise for the first two rows */
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            if (ok_first[r0][r1])
                dp[r0][r1] = 1;

    /* add rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear next */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                next[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr) {
                seq_t cur = dp[prev][curr];
                if (cur == 0) continue;
                for (int nxt = 0; nxt < 16; ++nxt)
                    if (ok_mid[prev][curr][nxt])
                        next[curr][nxt] += cur;
            }

        /* copy back */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = next[i][j];
    }

    /* final row check */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            if (dp[prev][curr] && ok_last[curr][prev])
                result += dp[prev][curr];

    return result;
}
