/*
 * A202644  Number of (n+2)X7 binary arrays avoiding patterns 000 and 111 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   build_rows_and_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A202644: number of (n+2)×7 binary arrays avoiding
   patterns 000 and 111 in rows, columns, and northwest‑to‑southeast
   diagonals.  The implementation precomputes all legal 7‑bit rows
   (42 patterns) and the transition matrix for triples of consecutive
   rows, then uses a standard DP over the rows.  The function works
   for any seq_t and idx_t types supplied by the build. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_rows_and_good.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    static uint8_t rows[42];
    static bool good[42][42][42];
    static bool initialized = false;
    static size_t hsize;

    if (!initialized) {
        build_rows_and_good(rows, &hsize, good);
        initialized = true;
    }

    /* Number of rows in the array */
    int m = (int)n + 2;          /* n >= 1 so m >= 3 */

    /* DP arrays: dp[i][j] = number of ways ending with rows[i], rows[j] */
    seq_t dp[42][42];
    seq_t newdp[42][42];

    /* Initialize with all first two rows legal */
    for (size_t i = 0; i < hsize; ++i)
        for (size_t j = 0; j < hsize; ++j)
            dp[i][j] = (seq_t)1;

    /* Transition for each additional row */
    for (int step = 1; step <= m - 2; ++step) {
        /* clear newdp */
        for (size_t i = 0; i < hsize; ++i)
            for (size_t j = 0; j < hsize; ++j)
                newdp[i][j] = (seq_t)0;

        for (size_t i = 0; i < hsize; ++i) {
            for (size_t j = 0; j < hsize; ++j) {
                seq_t cur = dp[i][j];
                if (cur == (seq_t)0) continue;
                for (size_t k = 0; k < hsize; ++k) {
                    if (good[i][j][k]) {
                        newdp[j][k] += cur;
                    }
                }
            }
        }

        /* copy newdp back to dp */
        for (size_t i = 0; i < hsize; ++i)
            for (size_t j = 0; j < hsize; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over all final states */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < hsize; ++i)
        for (size_t j = 0; j < hsize; ++j)
            total += dp[i][j];

    return total;
}
