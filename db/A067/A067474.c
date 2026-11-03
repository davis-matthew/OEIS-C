/*
 * A067474  Smallest n-digit square starting with 4.
 * https://oeis.org/A067474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ceil(sqrt(4 * 10^(n-1)))^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    /* n >= 1 according to the OEIS definition. */
    idx_t exp  = n - 1;                           /* exponent for 10^(n-1)   */
    seq_t target = 4 * pow10(exp);                /* 4 * 10^(n-1)            */
    seq_t r = floor_sqrt(target);                 /* floor(sqrt(target))     */
    if (r * r < target)                          /* round up if necessary   */
        r = r + 1;
    return r * r;                                 /* square the ceiling      */
}
