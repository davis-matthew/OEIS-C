/*
 * A189387  a(n) = n+[nr/s]+[nt/s]; r=1, s=sqrt(2), t=1/sqrt(3).
 * https://oeis.org/A189387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/sqrt(2)) + floor(n/sqrt(6))
 * Range:     u8 n <= 11, u16 n <= 84, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 100 terms through n = 4558 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed exactly using integer arithmetic:
   floor(n/sqrt(2)) = floor( sqrt( floor(n*n/2) ) )
   floor(n/sqrt(6)) = floor( sqrt( floor(n*n/6) ) )
   The helper floor_sqrt provides the required integer square root. */

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
    /* Convert index to seq_t for arithmetic. */
    seq_t nn = (seq_t)n * (seq_t)n;

    /* floor(n / sqrt(2)) = floor( sqrt( floor(n*n/2) ) ) */
    seq_t t1 = floor_sqrt( nn / 2 );

    /* floor(n / sqrt(6)) = floor( sqrt( floor(n*n/6) ) ) */
    seq_t t2 = floor_sqrt( nn / 6 );

    return (seq_t)n + t1 + t2;
}
