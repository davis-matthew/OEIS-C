/*
 * A180498  a(n) = n^2 - 5*floor(n/sqrt(5))^2.
 * https://oeis.org/A180498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - 5*floor(n/sqrt(5))^2
 * Range:     u8 n <= 15, u16 n <= 287, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n2 = (seq_t)n * (seq_t)n;      /* n^2 as seq_t */
    seq_t k  = floor_sqrt(n2 / 5);       /* floor(n / sqrt(5)) = floor(sqrt(n^2/5)) */
    return n2 - 5 * k * k;               /* n^2 - 5*k^2 */
}
