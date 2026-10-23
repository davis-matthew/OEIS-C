/*
 * A184583  floor[(n-1/5)(1+r)], where r=(1+sqrt(5))/2; complement of A184582.
 * https://oeis.org/A184583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n - 1/5) * (1 + r)) where r = (1+sqrt(5))/2
 * Range:     u8 none, u16 n <= 11, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            // offset starts at 1

    seq_t B = 5 * n - 1;             // m = 5n - 1
    seq_t sqrt_term = floor_sqrt((seq_t)5 * B * B);  // floor(B * sqrt5)
    seq_t numerator = (seq_t)3 * B + sqrt_term;     // 3B + floor(B*sqrt5)
    return numerator / 10;          // floor((3B + B*sqrt5)/10)
}
