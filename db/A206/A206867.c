/*
 * A206867  Number of nX4 0..1 arrays avoiding 0 0 0 horizontally and 1 0 1
 *          vertically.
 * https://oeis.org/A206867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A206867 via dynamic programming across rows.
   Each row is one of the 13 binary patterns of length 4 that avoid
   the horizontal substring 000.  A transition from pair (p,q) to a
   new row r is allowed iff for every column j the bits do not form
   the forbidden vertical pattern 1 0 1.  The algorithm counts
   sequences of length n by iterating these transitions. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, not used */
    if (n == 1) return (seq_t)13;          /* 13 admissible rows */

    /* All binary patterns of length 4 that avoid 000 horizontally. */
    static const uint8_t patterns[13] = {
        2,3,4,5,6,7,9,10,11,12,13,14,15   /* 0000,0001,1000 are excluded */
    };

    /* Pre‑compute which triples of patterns are compatible. */
    static bool good[13][13][13];
    static bool init = false;
    if (!init) {
        for (int p = 0; p < 13; ++p) {
            for (int q = 0; q < 13; ++q) {
                for (int r = 0; r < 13; ++r) {
                    bool ok = true;
                    uint8_t a = patterns[p];
                    uint8_t b = patterns[q];
                    uint8_t c = patterns[r];
                    for (int col = 0; col < 4; ++col) {
                        if (((a >> col) & 1) &&
                            !((b >> col) & 1) &&
                            ((c >> col) & 1)) {
                            ok = false;
                            break;
                        }
                    }
                    good[p][q][r] = ok;
                }
            }
        }
        init = true;
    }

    /* DP arrays: dp[p][q] holds counts ending with last two rows
       patterns indexed by p (row i‑1) and q (row i). */
    seq_t dp[13][13];
    seq_t nxt[13][13];
    /* initialise dp for first two rows: all pairs are allowed */
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 13; ++j)
            dp[i][j] = (seq_t)1;

    /* Build rows 3..n */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero nxt */
        for (int i = 0; i < 13; ++i)
            for (int j = 0; j < 13; ++j)
                nxt[i][j] = (seq_t)0;
        /* update transitions */
        for (int p = 0; p < 13; ++p) {
            for (int q = 0; q < 13; ++q) {
                seq_t cur = dp[p][q];
                if (cur == (seq_t)0) continue;
                for (int r = 0; r < 13; ++r) {
                    if (good[p][q][r]) {
                        nxt[q][r] = nxt[q][r] + cur;
                    }
                }
            }
        }
        /* copy nxt back to dp */
        for (int i = 0; i < 13; ++i)
            for (int j = 0; j < 13; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* sum over all ending pairs to obtain the total number of arrays */
    seq_t total = (seq_t)0;
    for (int p = 0; p < 13; ++p)
        for (int q = 0; q < 13; ++q)
            total = total + dp[p][q];

    return total;
}
