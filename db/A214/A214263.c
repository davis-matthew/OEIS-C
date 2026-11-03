/*
 * A214263  Expansion of f(x^1, x^7) in powers of x where f() is Ramanujan's
 *          general theta function.
 * https://oeis.org/A214263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if and only if n can be written as 4*k^2 - 3*k for some
 *            integer k (equivalently, if 9 + 16*n is a perfect square s with
 *            s ≡ 3 or 5 (mod 8)); otherwise a(n) = 0
 * Range:     u8 n <= 3, u16 n <= 971, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9180 (2026-11-01)
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
    /* Compute discriminant D = 9 + 16*n.  */
    seq_t D = (seq_t)9 + (seq_t)16 * (seq_t)n;

    /* Integer square root of D.  */
    seq_t s = floor_sqrt(D);

    /* Check for perfect square.  */
    if (s * s != D)
        return (seq_t)0;

    /* Check congruence condition: s ≡ 3 or 5 (mod 8).  */
    seq_t mod8 = s & (seq_t)7;  /* same as s % 8 for unsigned */
    if (mod8 == 3 || mod8 == 5)
        return (seq_t)1;
    return (seq_t)0;
}
