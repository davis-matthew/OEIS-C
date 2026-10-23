/*
 * A183860  a(n) = n+floor(sqrt(3n+1)); complement of A183861.
 * https://oeis.org/A183860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(3n+1))
 * Range:     u8 n <= 20, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t t   = ns * 3 + 1;     /* 3*n + 1 */
    seq_t s   = floor_sqrt(t);  /* floor(sqrt(3n+1)) */
    return ns + s;              /* n + floor(sqrt(3n+1)) */
}
