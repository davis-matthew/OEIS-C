/*
 * A128549  Difference between triangular number and next perfect square.
 * https://oeis.org/A128549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (floor(sqrt(n(n+1)/2))+1)^2 - n(n+1)/2
 * Range:     u8 n <= 10, u16 n <= 77, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t T  = nn * (nn + 1) / 2;          /* triangular number */
    seq_t s  = floor_sqrt(T) + 1;          /* floor sqrt + 1 */
    return s * s - T;                      /* difference to the next square */
}
