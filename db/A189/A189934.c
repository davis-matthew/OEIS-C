/*
 * A189934  a(n) = A189932(n)/2.
 * https://oeis.org/A189934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + floor((2*n/5)*sqrt(5)) - Equivalent to a(n) = (n +
 *            floor(n*(csc(pi/5))^2) + floor(n*(cot(pi/5))^2))/2
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 6758, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9659 (2026-11-01)
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
    if (n < 1)
        return 0;  /* undefined for n < 1, but return 0 as safe placeholder */

    seq_t nn = (seq_t)n;
    seq_t p = nn * (seq_t)2;            /* p = 2*n */
    seq_t A = p * p;                   /* p^2 */
    A *= (seq_t)5;                     /* A = 5 * p^2 */

    seq_t s = floor_sqrt(A);           /* floor(sqrt(5 * (2n)^2)) */
    seq_t t = s / (seq_t)5;            /* floor(sqrt(5 * (2n)^2) / 5) == floor((2n/5)*sqrt(5)) */

    return nn * (seq_t)2 + t;          /* a(n) = 2*n + t */
}
