/*
 * A189190  Number of nX3 binary arrays without the pattern 0 0 1 vertically
 *          or antidiagonally.
 * https://oeis.org/A189190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   is_allowed_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of n×3 binary arrays avoiding the pattern 0 0 1 vertically
   or antidiagonally.  Initial terms: a(1)=8, a(2)=64.
   The function computes the count by dynamic programming on 2‑row states. */
#include <stdint.h>
#include "../utils/is_allowed_triple.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, unused case */
    if (n == 1) return 8;
    if (n == 2) return 64;

    /* Precompute allowed transitions from state (p,q) to r. */
    static unsigned char allowed[8][8] = {0};
    static int ready = 0;
    if (!ready) {
        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                unsigned char mask = 0;
                for (int r = 0; r < 8; ++r) {
                    if (is_allowed_triple(p, q, r))
                        mask |= (1U << r);
                }
                allowed[p][q] = mask;
            }
        }
        ready = 1;
    }

    /* DP arrays for the last two rows. */
    seq_t dp_prev[8][8], dp_cur[8][8];

    /* Base case for i = 2: all pairs of rows are allowed. */
    for (int p = 0; p < 8; ++p)
        for (int q = 0; q < 8; ++q)
            dp_prev[p][q] = 1;

    for (idx_t i = 3; i <= n; ++i) {
        /* Zero the next state array. */
        for (int q = 0; q < 8; ++q)
            for (int r = 0; r < 8; ++r)
                dp_cur[q][r] = 0;

        /* Transition from (p,q) to (q,r). */
        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                seq_t val = dp_prev[p][q];
                if (val == 0) continue;
                unsigned char mask = allowed[p][q];
                for (int r = 0; r < 8; ++r) {
                    if (mask & (1U << r))
                        dp_cur[q][r] += val;
                }
            }
        }

        /* Copy dp_cur into dp_prev for next iteration. */
        for (int p = 0; p < 8; ++p)
            for (int q = 0; q < 8; ++q)
                dp_prev[p][q] = dp_cur[p][q];
    }

    /* Sum all sequences of length n. */
    seq_t total = 0;
    for (int p = 0; p < 8; ++p)
        for (int q = 0; q < 8; ++q)
            total += dp_prev[p][q];

    return total;
}
