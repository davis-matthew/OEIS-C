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
 * Validated: standard, pass, 33 terms through n = 200 (2026-11-01)
 * Helpers:   none
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

/* Build all horizontally legal 7‑bit rows and the validity table
   for triples (prev, curr, next).  This function is called only once
   on first invocation of a(). */
static void build_rows_and_good(uint8_t *rows, size_t *hsize, bool good[42][42][42]) {
    size_t count = 0;
    for (int p = 0; p < 128; ++p) {
        bool ok = true;
        for (int j = 0; j < 5; ++j) {            /* check runs of 3 consecutive bits */
            int b0 = (p >> j)       & 1;
            int b1 = (p >> (j+1))   & 1;
            int b2 = (p >> (j+2))   & 1;
            if (b0 == b1 && b1 == b2) { ok = false; break; }
        }
        if (ok) rows[count++] = (uint8_t)p;
    }
    *hsize = count;   /* should be 42 */

    for (size_t i = 0; i < *hsize; ++i) {
        for (size_t j = 0; j < *hsize; ++j) {
            for (size_t k = 0; k < *hsize; ++k) {
                bool vertical_ok = true;
                for (int col = 0; col < 7; ++col) {
                    int a = (rows[i] >> col) & 1;
                    int b = (rows[j] >> col) & 1;
                    int c = (rows[k] >> col) & 1;
                    if (a == b && b == c) { vertical_ok = false; break; }
                }
                if (!vertical_ok) { good[i][j][k] = false; continue; }

                bool diag_ok = true;
                for (int col = 2; col < 7; ++col) {   /* NW‑SE diagonals of length 3 */
                    int a = (rows[i] >> (col-2)) & 1;
                    int b = (rows[j] >> (col-1)) & 1;
                    int c = (rows[k] >> col)     & 1;
                    if (a == b && b == c) { diag_ok = false; break; }
                }
                good[i][j][k] = diag_ok;
            }
        }
    }
}

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
