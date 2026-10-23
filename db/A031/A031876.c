/*
 * A031876  a(n) = Sum_{k=0..n} floor(k^(1/3)).
 * https://oeis.org/A031876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} floor(k^(1/3))
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses int_cube_root from utils */

#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        sum += (seq_t)int_cube_root(k);
    }
    return sum;
}
