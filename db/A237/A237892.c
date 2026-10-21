/*
 * A237892  Number of (n+1)X(2+1) 0..3 arrays with the maximum minus the upper
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   max_minus_second
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: counts (n+1)×3 arrays of 0..3 whose every 2×2 subblock
   satisfies that the maximum minus the upper median (second largest)
   is a constant value d (0 ≤ d ≤ 3).  For each d we build the directed graph
   of 64 possible rows; an edge r→s exists iff the two 2×2 subblocks
   formed by rows r and s both have max–upper‑median equal to d.
   The number of valid arrays of height n+1 is the number of walks of
   length n in that graph.  The total over all d gives a(n);  we
   compute it by dynamic programming. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/max_minus_second.h"

/* convert a row index 0..63 to its three base‑4 digits */
static inline void decode_row(uint8_t idx, uint8_t *a0, uint8_t *a1, uint8_t *a2)
{
    *a0 = idx & 3;
    *a1 = (idx >> 2) & 3;
    *a2 = (idx >> 4) & 3;
}

seq_t a(idx_t n)
{
    /* adjacency[d][r][s] is true if rows r and s can follow each other
       with constant d.  Dimensions: 4×64×64 */
    static bool adjacency[4][64][64];
    static bool initialized = false;

    if (!initialized) {
        /* precompute adjacency once */
        uint8_t rows0[64], rows1[64], rows2[64];
        for (int r = 0; r < 64; ++r) {
            rows0[r] = r & 3;
            rows1[r] = (r >> 2) & 3;
            rows2[r] = (r >> 4) & 3;
        }
        for (int r = 0; r < 64; ++r) {
            for (int s = 0; s < 64; ++s) {
                /* compute the two 2×2 subblocks */
                uint8_t diff1 = max_minus_second(rows0[r], rows1[r], rows0[s], rows1[s]);
                uint8_t diff2 = max_minus_second(rows1[r], rows2[r], rows1[s], rows2[s]);
                if (diff1 == diff2) {
                    adjacency[diff1][r][s] = true;
                }
            }
        }
        initialized = true;
    }

    if (n == 0) return 0;          /* not defined for n=0, but harmless */

    seq_t result = 0;                /* accumulate total over all d */

    /* For each possible constant d, count walks of length n in its graph */
    for (int d = 0; d < 4; ++d) {
        seq_t dp_prev[64], dp_cur[64];

        /* initial state: any first row is possible */
        for (int i = 0; i < 64; ++i) dp_prev[i] = 1;

        for (idx_t step = 1; step <= n; ++step) {
            /* clear current */
            for (int j = 0; j < 64; ++j) dp_cur[j] = 0;

            /* transition */
            for (int i = 0; i < 64; ++i) {
                seq_t val = dp_prev[i];
                if (val == 0) continue;
                for (int j = 0; j < 64; ++j) {
                    if (adjacency[d][i][j]) dp_cur[j] += val;
                }
            }

            /* advance */
            for (int j = 0; j < 64; ++j) dp_prev[j] = dp_cur[j];
        }

        /* sum all final states */
        for (int j = 0; j < 64; ++j) result += dp_prev[j];
    }

    return result;
}
