/*
 * A159684  Sturmian word: limit S(infinity) where S(0) = 0, S(1) = 0,1 and
 *          for n>=1, S(n+1) = S(n)S(n)S(n-1).
 * https://oeis.org/A159684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)*(sqrt(2)-1)) - floor((n+1)*(sqrt(2)-1))
 * Range:     u8 n <= 3, u16 n <= 88, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9365 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the identity floor(k*(sqrt(2)-1)) = floor(k*sqrt(2)) - k,
   and floor(k*sqrt(2)) = floor_sqrt(2*k*k).  */
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

seq_t a(idx_t n) {
    idx_t n1 = n + 1;
    idx_t n2 = n + 2;

    /* Compute floor(k*sqrt(2)) for k = n1 and n2 */
    seq_t k1_sqr = (seq_t)n1 * (seq_t)n1;
    seq_t k2_sqr = (seq_t)n2 * (seq_t)n2;

    seq_t v1 = floor_sqrt( (seq_t)2 * k1_sqr );   /* floor((n+1)*sqrt(2)) */
    seq_t v2 = floor_sqrt( (seq_t)2 * k2_sqr );   /* floor((n+2)*sqrt(2)) */

    /* a(n) = (v2 - n2) - (v1 - n1) = v2 - v1 - 1 */
    return v2 - v1 - 1;
}
