/*
 * A182761  Beatty sequence for (7 - sqrt(5))/2.
 * https://oeis.org/A182761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(7 - sqrt(5))/2)
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 7849 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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
    /* n >= 1 */
    seq_t nn = (seq_t)n;

    /* floor(n*sqrt(5)) via integer square root of 5*n*n */
    seq_t sqrt5_floor = floor_sqrt(nn * nn * 5);

    /* A = 7*n - floor(n*sqrt(5)) */
    seq_t A = 7 * nn - sqrt5_floor;

    /* floor(A/2) */
    seq_t base = A >> 1;           /* since A is unsigned, right shift is div by 2 */

    /* If A is even we need to subtract 1, otherwise keep base */
    return (A & 1) ? base : (base - 1);
}
