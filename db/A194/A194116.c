/*
 * A194116  a(n) = Sum_{j=1..n} floor(j*sqrt(13)); n-th partial sum of Beatty
 *          sequence for sqrt(13).
 * https://oeis.org/A194116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} floor(j*sqrt(13))
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t t = (seq_t)j * (seq_t)j;   /* j^2 */
        t *= (seq_t)13;                  /* 13 * j^2 */
        sum += floor_sqrt(t);            /* floor(sqrt(13*j^2)) = floor(j*sqrt(13)) */
    }
    return sum;
}
