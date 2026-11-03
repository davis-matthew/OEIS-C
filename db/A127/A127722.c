/*
 * A127722  Floor of square root of sum of squares of n consecutive odd
 *          numbers.
 * https://oeis.org/A127722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( n*(4n^2-1)/3 ) )
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
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
    if (n == 0) return 0;                     /* offset is 1, but handles n=0 safely */
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;                      /* n^2 */
    seq_t t = nn2 * 4;                        /* 4n^2 */
    t -= 1;                                   /* 4n^2 - 1 */
    seq_t val;
    if ((n % 3) == 0)                         /* n divisible by 3 → divide n first */
        val = (nn / 3) * t;                   /* (n/3) * (4n^2-1) */
    else
        val = nn * (t / 3);                   /* n * ((4n^2-1)/3) */
    return floor_sqrt(val);
}
