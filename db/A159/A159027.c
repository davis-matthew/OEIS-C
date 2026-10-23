/*
 * A159027  a(0)=109; a(n) = a(n-1) + floor(sqrt(a(n-1))), n > 0.
 * https://oeis.org/A159027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 201, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=109; a(n)=a(n-1)+floor_sqrt(a(n-1)) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)109;   /* a(0) */

    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t s = floor_sqrt(val);
        val += s;
    }
    return val;
}
