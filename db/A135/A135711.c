/*
 * A135711  Minimal perimeter of a polyhex with n cells.
 * https://oeis.org/A135711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*ceil(sqrt(12*n-3))
 * Range:     u8 n <= 5, u16 n <= 1118, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but harmless */
    seq_t x = (seq_t)12 * (seq_t)n - 3; /* 12*n - 3 */
    seq_t s = floor_sqrt(x);          /* floor(sqrt(x)) */
    if (s * s < x) s++;               /* ceil(sqrt(x)) */
    return 2 * s;                     /* final perimeter */
}
