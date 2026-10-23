/*
 * A184814  n+[nr/t]+[ns/t], where r=sqrt(2), s=sqrt(3), t=sqrt(5), and
 *          []=floor.
 * https://oeis.org/A184814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)/sqrt(5)) + floor(n*sqrt(3)/sqrt(5))
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 2412, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t N1 = nn * nn * (seq_t)10;
    seq_t s1 = sqrt_floor(N1);
    seq_t term1 = s1 / (seq_t)5;
    seq_t N2 = nn * nn * (seq_t)15;
    seq_t s2 = sqrt_floor(N2);
    seq_t term2 = s2 / (seq_t)5;
    return nn + term1 + term2;
}
