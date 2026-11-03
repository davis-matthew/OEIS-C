/*
 * A186539  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-2+3j^2. Complement
 *          of A186540.
 * https://oeis.org/A186539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt((1/3) n^2 + 1/24)) (OEIS A186539)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
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

seq_t a(idx_t n) {
    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;

    /* Compute n^2 */
    seq_t x = nn * nn;

    /* numerator = 8 * n^2 + 1  */
    seq_t num = x * (seq_t)8 + (seq_t)1;

    /* t = floor((8*n^2 + 1) / 24) */
    seq_t t = num / (seq_t)24;

    /* k = floor(sqrt(t)) */
    seq_t k = floor_sqrt(t);

    /* Return a(n) = n + k */
    return nn + k;
}
