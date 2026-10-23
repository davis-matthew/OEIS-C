/*
 * A182617  Number of toothpicks in a toothpick spiral around n cells on
 *          hexagonal net.
 * https://oeis.org/A182617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*n + ceil(sqrt(12*n - 3)) for n > 0, a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t x   = (seq_t)12 * nn - (seq_t)3;      /* 12*n - 3 */
    seq_t s   = floor_sqrt(x);                 /* floor(sqrt(12*n - 3)) */
    seq_t ceil_sqrt = s;

    if (s * s < x)            /* not a perfect square -> ceil */
        ceil_sqrt = s + (seq_t)1;

    return (seq_t)2 * nn + ceil_sqrt;
}
