/*
 * A047077  Number of pairs of integers (x,y) with n^2 <= x^2+y^2 <= (n+1)^2.
 * https://oeis.org/A047077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = number of integer pairs (x,y) with n^2 <= x^2 + y^2 <=
 *            (n+1)^2
 * Range:     u8 n <= 6, u16 n <= 257, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    seq_t sn = (seq_t)n;
    seq_t sn_plus1 = sn + (seq_t)1;
    seq_t inner_sq = sn * sn;
    seq_t outer_sq = sn_plus1 * sn_plus1;
    idx_t x_max = n + (idx_t)1;

    for (idx_t ix = 0; ix <= x_max; ++ix) {
        seq_t x = (seq_t)ix;
        seq_t x_sq = x * x;

        /* y_max^2 = outer_sq - x^2 */
        if (outer_sq < x_sq)
            continue;
        seq_t y_max_sq = outer_sq - x_sq;
        seq_t y_max = floor_sqrt(y_max_sq);

        /* y_min^2 = max(0, inner_sq - x^2) */
        seq_t y_min_sq;
        if (inner_sq > x_sq)
            y_min_sq = inner_sq - x_sq;
        else
            y_min_sq = (seq_t)0;

        if (y_min_sq > y_max_sq)
            continue;

        seq_t floor_val = floor_sqrt(y_min_sq);
        seq_t y_min = floor_val;
        if (y_min * y_min < y_min_sq)
            ++y_min;

        if (y_min > y_max)
            continue;

        /* Count points for this x */
        bool y0_in_range = (y_min == (seq_t)0);
        seq_t pos_start = y_min;
        if (pos_start < (seq_t)1)
            pos_start = (seq_t)1;
        seq_t pos_count = 0;
        if (pos_start <= y_max)
            pos_count = y_max - pos_start + (seq_t)1;

        if (ix == 0) {
            result += (seq_t)y0_in_range * (seq_t)1 + pos_count * (seq_t)2;
        } else {
            result += (seq_t)y0_in_range * (seq_t)2 + pos_count * (seq_t)4;
        }
    }

    return result;
}
