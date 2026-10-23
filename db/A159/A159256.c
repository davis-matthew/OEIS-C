/*
 * A159256  a(0)=131; for n > 0, a(n) = a(n-1) + floor(sqrt(a(n-1))).
 * https://oeis.org/A159256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 131; for n > 0, a(n) = a(n-1) + floor(sqrt(a(n-1)))
 * Range:     u8 n <= 0, u16 n <= 164, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)131;                  /* a(0) */

    for (idx_t i = 1; i <= n; ++i) {        /* compute a(1) … a(n) */
        seq_t inc = floor_sqrt(val);        /* floor(sqrt(a(i-1))) */
        val += inc;                         /* a(i) = a(i-1) + inc */
    }

    return val;                              /* a(n) */
}
