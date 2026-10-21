/*
 * A072945  Number of pairs (x,y) with x,y >= 0 such that x^3+y^2+x*y <= n.
 * https://oeis.org/A072945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y) ≥ 0 with x³ + y² + x·y ≤ n
 * Range:     u8 n <= 15, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    idx_t max_x = int_cube_root(n);
    seq_t total = 0;
    for (idx_t x = 0; x <= max_x; ++x)
    {
        seq_t sub   = (seq_t)n - (seq_t)x * (seq_t)x * (seq_t)x;
        seq_t disc  = (seq_t)x * (seq_t)x + (seq_t)4 * sub;
        seq_t sqrt_disc = floor_sqrt(disc);
        seq_t y_max = (sqrt_disc - (seq_t)x) / 2;
        total += y_max + 1;
    }
    return total;
}
