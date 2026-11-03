/*
 * A087060  Difference between 2n^2 and the nearest square number.
 * https://oeis.org/A087060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = min [ 2*n^2 - floor(n*sqrt(2))^2, (1 +
 *            floor(n*sqrt(2)))^2 - 2*n^2 ]
 * Range:     u8 n <= 5, u16 n <= 78, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9653 (2026-11-01)
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
    seq_t ns = (seq_t)n;
    seq_t two_n2 = 2 * ns * ns;            /* 2 * n^2 */
    seq_t k = floor_sqrt(two_n2);         /* floor(n*sqrt(2)) */
    seq_t diff1 = two_n2 - k * k;         /* difference to lower square */

    seq_t kp1 = k + 1;                    /* ceil(n*sqrt(2)) */
    seq_t diff2 = kp1 * kp1 - two_n2;     /* difference to upper square */

    return (diff1 < diff2) ? diff1 : diff2;
}
