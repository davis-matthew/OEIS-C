/*
 * A235844  Number of (n+1)X(2+1) 0..3 arrays with the upper median unequal to
 *          the lower median in every 2X2 subblock.
 * https://oeis.org/A235844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   upper_lower_median_distinct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes a(n) for OEIS A235844.
   We model the problem as a DP on the rows of a (n+1)×3 array.
   Each row has 3 digits (0…3).  Two consecutive rows are allowed to
   follow one another iff for both overlapping 2×2 blocks the upper and
   lower medians are distinct.  There are 4^3 = 64 possible rows,
   so the DP runs over a 64×64 adjacency matrix.  For n ≥ 1 the
   number of arrays equals the number of length‑(n+1) walks in this
   directed graph.  The implementation uses a simple O(n·64²)
   algorithm that works for any integer type seq_t and idx_t.  */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/upper_lower_median_distinct.h"

seq_t a(idx_t n)
{
    /* On first call build the adjacency matrix and the digit tables. */
    static bool tables_ready = false;
    static unsigned char digits[64][3];
    static unsigned char adj[64][64];

    if (!tables_ready) {
        for (int r = 0; r < 64; ++r) {
            digits[r][0] = r / 16;
            digits[r][1] = (r / 4) % 4;
            digits[r][2] = r % 4;
        }
        for (int r = 0; r < 64; ++r) {
            for (int s = 0; s < 64; ++s) {
                int r0 = digits[r][0], r1 = digits[r][1], r2 = digits[r][2];
                int s0 = digits[s][0], s1 = digits[s][1], s2 = digits[s][2];
                adj[r][s] = (unsigned char)(
                    upper_lower_median_distinct(r0, r1, s0, s1) &&
                    upper_lower_median_distinct(r1, r2, s1, s2));
            }
        }
        tables_ready = true;
    }

    /* DP arrays: prev holds counts for sequences ending in each row
       after k transitions; cur will store the next step. */
    seq_t prev[64];
    seq_t cur[64];

    /* Initial state: one array of length 1 (single row) for each of the 64 rows. */
    for (int r = 0; r < 64; ++r) {
        prev[r] = 1;
    }

    /* Perform n transitions → produce arrays of (n+1) rows. */
    for (idx_t step = 0; step < n; ++step) {
        for (int s = 0; s < 64; ++s) {
            cur[s] = (seq_t)0;
        }
        for (int r = 0; r < 64; ++r) {
            seq_t counts = prev[r];
            if (counts == (seq_t)0) continue;
            unsigned char *adj_row = adj[r];
            for (int s = 0; s < 64; ++s) {
                if (adj_row[s]) {
                    cur[s] += counts;
                }
            }
        }
        /* Copy cur into prev for the next iteration. */
        for (int s = 0; s < 64; ++s) {
            prev[s] = cur[s];
        }
    }

    /* Sum over all possible last rows to obtain the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int r = 0; r < 64; ++r) {
        total += prev[r];
    }
    return total;
}
