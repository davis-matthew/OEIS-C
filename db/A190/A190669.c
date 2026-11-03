/*
 * A190669  a(n) = [(b*n+c)*r] - b*[n*r] - [c*r], where (r,b,c)=(sqrt(3),2,0)
 *          and [ ] = floor.
 * https://oeis.org/A190669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2*n*sqrt(3)) - 2*floor(n*sqrt(3))
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9369, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 148 terms through n = 9369 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: use integer sqrt: floor(n*sqrt(3)) = floor( sqrt(3*n²) ) */
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
    seq_t n_seq = (seq_t)n;     /* cast to seq_t once */
    seq_t n2    = n_seq * n_seq;            /* n² */
    seq_t val1  = n2 * (seq_t)3;            /* 3*n² */
    seq_t sqrt1 = floor_sqrt(val1);         /* floor(n*sqrt(3)) */
    seq_t val2  = n2 * (seq_t)12;           /* 12*n² = (2n)²*3 */
    seq_t sqrt2 = floor_sqrt(val2);         /* floor(2*n*sqrt(3)) */
    return sqrt2 - (sqrt1 << 1);            /* sqrt2 - 2*sqrt1 */
}
