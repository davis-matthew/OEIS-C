/*
 * A190057  a(n) = n + [n*r/s] + [n*t/s]; r=1/2, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(n*n / 3)) + floor(4*n / 3)
 * Range:     u8 n <= 9, u16 n <= 428, u32 n <= 7891, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t term = sn
               + sqrt_floor( (sn * sn) / (seq_t)3 )
               + (sn * (seq_t)4) / (seq_t)3;
    return term;
}
