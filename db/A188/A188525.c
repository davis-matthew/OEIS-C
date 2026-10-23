/*
 * A188525  a(n) = rad(rad(n)^2+1), where rad = A007947.
 * https://oeis.org/A188525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rad(rad(n)^2+1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    seq_t r = rad(n);          /* rad of n */
    seq_t y = r * r + 1;       /* rad(n)^2 + 1 */
    return rad((idx_t)y);      /* rad of that value */
}
