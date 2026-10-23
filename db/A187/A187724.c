/*
 * A187724  Number of (n+1)X5 binary arrays with no 2X2 subblock commuting
 *          with any of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A187724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   check_horiz, check_vertical, commute, get_matrix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: transfer‑matrix enumeration of binary arrays
   satisfying the non‑commutation constraints. This is not a simple
   formula, but the algorithm enumerates row patterns via DP. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/check_horiz.h"
#include "../utils/check_vertical.h"
#include "../utils/commute.h"
#include "../utils/get_matrix.h"

seq_t a(idx_t n) {
    /* number of rows in the array = n+1 */
    idx_t rows = n + 1;

    /* static tables of admissible horizontal and vertical pairs */
    static bool H[32][32];
    static bool V[32][32][32];
    static bool tables_inited = false;
    if (!tables_inited) {
        tables_inited = true;
        for (uint8_t a = 0; a < 32; ++a)
            for (uint8_t b = 0; b < 32; ++b)
                H[a][b] = check_horiz(a, b);
        for (uint8_t a = 0; a < 32; ++a)
            for (uint8_t b = 0; b < 32; ++b)
                for (uint8_t c = 0; c < 32; ++c)
                    V[a][b][c] = check_vertical(a, b, c);
    }

    /* dynamic programming tables */
    seq_t dp[32][32];
    seq_t dp_next[32][32];

    /* initialise all entries to zero */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* initial row pair (rows 1 and 2) must satisfy horizontal constraint */
    for (uint8_t r1 = 0; r1 < 32; ++r1)
        for (uint8_t r2 = 0; r2 < 32; ++r2)
            if (H[r1][r2])
                dp[r1][r2] = 1;

    /* iterate over additional rows */
    for (idx_t step = 3; step <= rows; ++step) {
        /* zero dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (uint8_t prev = 0; prev < 32; ++prev) {
            for (uint8_t curr = 0; curr < 32; ++curr) {
                seq_t cnt = dp[prev][curr];
                for (uint8_t next = 0; next < 32; ++next) {
                    if (!H[curr][next])
                        continue;
                    if (!V[prev][curr][next])
                        continue;
                    dp_next[curr][next] += cnt;
                }
            }
        }

        /* move to next step */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* total count is sum over all final state pairs */
    seq_t total = 0;
    for (uint8_t i = 0; i < 32; ++i)
        for (uint8_t j = 0; j < 32; ++j)
            total += dp[i][j];

    return total;
}
