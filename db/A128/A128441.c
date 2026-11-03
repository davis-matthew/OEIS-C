/*
 * A128441  a(n)=Floor(n*2^(n/2)).
 * https://oeis.org/A128441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n * 2^(n/2) )
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

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
    /* For safety treat n == 0 as 0; the sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;

    if ((n & 1) == 0)            /* even n = 2k */
    {
        idx_t k = n >> 1;          /* k = n/2 */
        /* a = n * 2^k */
        return ((seq_t)n) * pow2(k);
    }
    else                          /* odd n = 2k + 1 */
    {
        idx_t k = n >> 1;          /* k = floor(n/2) */
        /* I = n * 2^k = (2k+1) * 2^k   */
        seq_t I = ((seq_t)n) * pow2(k);  

        /* Compute floor( sqrt( I^2 * 2 ) ) = floor( I * sqrt(2) ) = floor( n * 2^(n/2) ) */
        seq_t t = I * I;           /* I² */
        t = t + t;                 /* I² * 2 */
        return floor_sqrt(t);
    }
}
