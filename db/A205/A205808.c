/*
 * A205808  G.f.: Sum_{n=-oo..oo} q^(9*n^2 + 2*n).
 * https://oeis.org/A205808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 432, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 137 terms through n = 963 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A205808
   a(n) = 1 if n can be written as 9*k^2 + 2*k or as 9*k^2 + 16*k + 7
          for some integer k (positive, negative or zero).
   Otherwise a(n) = 0.
*/

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Compute discriminant D = 4 + 36*n */
    seq_t D = 4 + 36 * n;

    /* Integer square root of D */
    seq_t s = floor_sqrt(D);

    /* If D is not a perfect square, no integer k exists */
    if (s * s != D)
        return 0;

    /* Check the four possible roots:
       k = (-2 ± s)/18   (from 9*k^2 + 2*k = n)
       k = (-16 ± s)/18  (from 9*k^2 + 16*k + 7 = n)
    */
    /* First formula: (-2 + s)/18 */
    if (s >= 2 && ((s - 2) % 18) == 0)
        return 1;
    /* First formula: (-2 - s)/18 */
    if ((s + 2) % 18 == 0)
        return 1;
    /* Second formula: (-16 + s)/18 */
    if (s >= 16 && ((s - 16) % 18) == 0)
        return 1;
    /* Second formula: (-16 - s)/18 */
    if ((s + 16) % 18 == 0)
        return 1;

    return 0;
}
