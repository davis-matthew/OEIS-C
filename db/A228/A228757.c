/*
 * A228757  Number of 5Xn binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or antidiagonally.
 * https://oeis.org/A228757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over column transitions for 5×n binary arrays
 *  - top-left cell is fixed to 1
 *  - no two 1’s may be horizontally adjacent (same row, consecutive columns)
 *  - no two 1’s may be antidiagonally adjacent (rows differ by 1, columns differ by 1)
 *
 *  While no closed‑form is known, a quick recurrence exists using a 32×32
 *  transition matrix (5 rows → 5 bits → 32 patterns).  The DP runs in
 *  O(n·32²) which is well below the time limit for n≤10000.
 *
 *  This implementation is valid in both C11 and C++17; seq_t behaves
 *  as a simple unsigned integer type or an arbitrary‑precision class.
 *
 *  The function traverses all column patterns, propagating counts
 *  for each column.  The initial column requires the top bit set.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* offset in OEIS is 1; n=0 is unused */
    }

    /* Transition table: trans[p][q] == true iff pattern q can follow pattern p */
    static bool trans[32][32];
    static bool initialized = false;

    if (!initialized) {
        for (int p = 0; p < 32; ++p) {
            for (int q = 0; q < 32; ++q) {
                /* Horizontal adjacency: same row cannot both be 1 */
                if (p & q) {
                    trans[p][q] = false;
                    continue;
                }
                /* Antidiagonal adjacency: (row r in p) & (row r‑1 in q) cannot be 1 */
                if (p & (q << 1)) {
                    trans[p][q] = false;
                    continue;
                }
                trans[p][q] = true;
            }
        }
        initialized = true;
    }

    /* DP arrays for one column and the next */
    seq_t dp[32]   = {0};
    seq_t next[32] = {0};

    /* Initial column: top-left element must be 1 (bit 0) */
    for (int p = 0; p < 32; ++p) {
        if (p & 1) {
            dp[p] = 1;
        }
    }

    /* Process remaining columns */
    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < 32; ++i) {
            next[i] = 0;
        }
        for (int p = 0; p < 32; ++p) {
            if (dp[p] == 0) {
                continue;
            }
            for (int q = 0; q < 32; ++q) {
                if (trans[p][q]) {
                    next[q] += dp[p];
                }
            }
        }
        for (int i = 0; i < 32; ++i) {
            dp[i] = next[i];
        }
    }

    /* Sum over final column patterns */
    seq_t result = 0;
    for (int i = 0; i < 32; ++i) {
        result += dp[i];
    }
    return result;
}
