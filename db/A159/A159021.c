/*
 * A159021  a(0)=19; a(n) = a(n-1) + floor(sqrt(a(n-1))), n > 0.
 * https://oeis.org/A159021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor(sqrt(a(n-1))) with a(0) = 19
 * Range:     u8 n <= 9, u16 n <= 251, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t value = (seq_t)19;  /* a(0) = 19 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t step = floor_sqrt(value); /* floor(sqrt(a(i-1))) */
        value += step;
    }
    return value;
}
