/*
 * A195127  a(n) = 2*n - floor(n*r/5), where r = (1 + sqrt(5))/2.
 * https://oeis.org/A195127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - ⌊n*r/5⌋ with r = (1+√5)/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 7997 (2026-11-01)
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

/* Computes a(n) for n >= 0 */
seq_t a(idx_t n)
{
    /* Convert index to seq_t arithmetic */
    seq_t nn = (seq_t)n;

    /* floor(n*√5)  = floor( √(5*n²) ) */
    seq_t sqrt_term = floor_sqrt(5 * nn * nn);

    /* floor(n*r/5)  = ⌊(n + floor(n√5))/10⌋ */
    seq_t div_term = (nn + sqrt_term) / 10;

    /* a(n) = 2*n - floor(n*r/5) */
    return 2 * nn - div_term;
}
