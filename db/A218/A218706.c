/*
 * A218706  Number of nonnegative integer solutions to x^2 + 2y^2 <= n^2.
 * https://oeis.org/A218706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to * x^2 + 2y^2
 *            <= n^2 * * Compute by scanning all y with 2y^2 <= n^2 and adding
 *            * floor(sqrt(n^2 - 2y^2)) + 1 (for each admissible x)
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"    /* declares seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;     /* n^2 (fits in seq_t) */
    seq_t count = 0;

    for (idx_t y = 0;; ++y) {
        seq_t y_sq  = (seq_t)y * (seq_t)y;              /* y^2  */
        seq_t two_y_sq = (seq_t)2 * y_sq;               /* 2y^2 */

        if (two_y_sq > n_sq)            /* no further y possible */
            break;

        seq_t remaining = n_sq - two_y_sq;
        seq_t x_max = floor_sqrt(remaining);           /* max x for this y */
        count += x_max + 1;                            /* include x=0 */
    }

    return count;
}
