/*
 * A135034  Positive integers n repeated 2n-1 times, with a leading a(0) = 0.
 *          Also: ceiling of square root of n.
 * https://oeis.org/A135034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(sqrt(n))
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t m = floor_sqrt((seq_t)n);           /* floor(sqrt(n)) */
    uint64_t mm = (uint64_t)m * (uint64_t)m;   /* (floor(sqrt(n)))², safe in 64‑bit */
    if (mm == n) return m;                    /* perfect square */
    return m + 1;                             /* otherwise ceil */
}
