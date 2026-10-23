/*
 * A190055  a(n) = n + [n*r/t] + [n*s/t]; r=2, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3)) + floor((3*n)/4)
 * Range:     u8 n <= 8, u16 n <= 442, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt3_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt3_of_n.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t sqrt_part = floor_sqrt3_of_n(N);
    seq_t frac_part = (3 * N) / 4;
    return N + sqrt_part + frac_part;
}
