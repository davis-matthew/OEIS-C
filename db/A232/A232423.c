/*
 * A232423  a(n) = ceiling(sqrt(n^4 - n^3 - n^2 + n + 1))^2 - (n^4 - n^3 - n^2
 *          + n + 1).
 * https://oeis.org/A232423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(n^4 - n^3 - n^2 + n + 1))^2 - (n^4 - n^3 - n^2
 *            + n + 1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 53 (2026-11-01)
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
    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;          /* n^2 */
    seq_t n3  = n2 * nn;          /* n^3 */
    seq_t n4  = n2 * n2;          /* n^4 */
    seq_t f   = n4 - n3 - n2 + nn + 1; /* f(n) */

    seq_t s = floor_sqrt(f);           /* floor(sqrt(f)) */
    if (s * s == f)                    /* perfect square */
        return 0;

    seq_t t = (s + 1) * (s + 1);       /* next square */
    return t - f;                      /* a(n) */
}
