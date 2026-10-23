/*
 * A184675  a(n) = n + floor(sqrt(n) + sqrt(n+1)); complement of A184674.
 * https://oeis.org/A184675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(4*n + 1))
 * Range:     u8 n <= 15, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * ((seq_t)4) + 1;
    seq_t s = floor_sqrt(m);
    return (seq_t)n + s;
}
