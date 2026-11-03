/*
 * A206903  n+[ns/r]+[nt/r], where []=floor, r=3, s=sqrt(3), t=1/s.
 * https://oeis.org/A206903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·√3/3⌋ + ⌊n/(3√3)⌋
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: use integer arithmetic only: compute m = floor(sqrt(3·n²)),
   then a(n) = n + ⌊m/3⌋ + ⌊m/9⌋. */

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
    /* Convert the integer index to the arithmetic type. */
    seq_t n_val = n;                     /* n as seq_t */

    /* m = floor(sqrt(3 * n²)) = floor(n·√3).  This uses only integer
       operations. */
    seq_t m = floor_sqrt((seq_t)3 * n_val * n_val);

    /* Compute the final value using integer division. */
    seq_t term1 = m / 3;   /* floor(n·√3 / 3)  */
    seq_t term2 = m / 9;   /* floor(n·√3 / 9)  */

    return n_val + term1 + term2;
}
