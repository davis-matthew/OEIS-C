/*
 * A175367  A175366(n^2).
 * https://oeis.org/A175367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) with |x|^3+|y|^3+|z|^3
 *            <= n^2, -n <= x,y,z <= n
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t limit = (seq_t)n * (seq_t)n;          /* n^2 */

    for (idx_t a_idx = 0; ; ++a_idx)
    {
        idx_t ca8 = a_idx * a_idx * a_idx;     /* a^3 as idx_t to avoid overflow in intermediate */
        seq_t cube_a = (seq_t)ca8;
        if (cube_a > limit) break;
        seq_t w_a = (a_idx == 0 ? (seq_t)1 : (seq_t)2);

        for (idx_t b_idx = 0; ; ++b_idx)
        {
            idx_t cb8 = b_idx * b_idx * b_idx; /* b^3 as idx_t */
            seq_t cube_b = (seq_t)cb8;
            seq_t sum_ab = cube_a + cube_b;
            if (sum_ab > limit) break;
            seq_t w_b = (b_idx == 0 ? (seq_t)1 : (seq_t)2);

            seq_t remaining = limit - sum_ab;
            seq_t c_max = floor_cube_root(remaining);   /* floor(cuberoot(remaining)) */
            seq_t cnt_c = (seq_t)1 + (seq_t)2 * c_max;  /* sum of weights for c = 0..c_max */

            total += w_a * w_b * cnt_c;
        }
    }
    return total;
}
