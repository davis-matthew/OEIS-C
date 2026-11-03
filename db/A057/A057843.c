/*
 * A057843  a(n) = floor(n*tau^2) - 3, where tau = (1+sqrt(5))/2.
 * https://oeis.org/A057843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(n*tau^2) - 3
 * Range:     i8 n <= 3, i16 n <= 57, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  with  tau = (1+sqrt(5))/2   */
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
    if (n == 0) {            /* behaviour for offset < 1 is irrelevant */
        return 0;
    }

    seq_t nn = (seq_t)n;                 /* cast index to seq_t */

    /* floor(n*sqrt(5)) = floor(sqrt(5*n^2)) */
    seq_t sq = (seq_t)5 * nn * nn;
    seq_t sqrt_floor = floor_sqrt(sq);

    /* floor(n*phi) where phi = (1+sqrt(5))/2 */
    seq_t phi_floor = (nn + sqrt_floor) / 2;

    /* a(n) = floor(n*phi) + n - 3  */
    return phi_floor + nn - (seq_t)3;
}
