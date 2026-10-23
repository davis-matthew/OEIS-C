/*
 * A048468  a(n) = T(7,n), array T given by A047858.
 * https://oeis.org/A048468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * (n+14) - 6 for n>=1, with a(0)=1
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t val = pow2(n - 1);           /* 2^(n-1) */
    seq_t mult = (seq_t)n + (seq_t)14; /* (n+14) as seq_t */
    val = val * mult;                  /* 2^(n-1) * (n+14) */
    val = val - (seq_t)6;              /* subtract 6 */
    return val;
}
