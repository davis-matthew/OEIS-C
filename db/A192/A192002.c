/*
 * A192002  Counting sequence for Wythoff AB-numbers smaller than n.
 * https://oeis.org/A192002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = z(n)+z(n-1)-n, where z(n) = floor((n+1)/phi), phi =
 *            (1+sqrt(5))/2. The computation is performed using only integer
 *            arithmetic. z(n) is evaluated exactly via the identity z(n) =
 *            floor( ( floor_sqrt(5*(n+1)^2) - (n+1) ) / 2 ). floor_sqrt is
 *            provided by the build utilities
 * Range:     u8 n <= 2, u16 n <= 56, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
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

/* Compute z(n) = floor((n+1)/phi) using integer arithmetic */
static seq_t z_of(idx_t n)
{
    seq_t m = (seq_t)n + 1;          /* m = n+1 */
    seq_t t = (seq_t)5 * m * m;      /* t = 5*m*m */
    seq_t s = floor_sqrt(t);         /* s = floor(sqrt(t)) = floor(m*sqrt(5)) */
    return (s - m) / 2;              /* z = floor((s - m)/2) */
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    seq_t zn  = z_of(n);             /* z(n)   */
    seq_t zn1 = z_of(n - 1);         /* z(n-1) */
    return zn + zn1 - (seq_t)n;      /* a(n) = z(n)+z(n-1)-n */
}
