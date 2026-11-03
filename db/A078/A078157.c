/*
 * A078157  a(n) = A078152(2^n).
 * https://oeis.org/A078157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(0, floor(sqrt(2^(n+2))) - n - 2)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
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
    /* 2^(n+2) as a big integer */
    seq_t exp  = (seq_t)(n + 2);
    seq_t base = (seq_t)2;
    seq_t power = ipow(base, exp);

    /* floor(√(2^(n+2))) */
    seq_t sqrtv = floor_sqrt(power);

    /* n + 2 */
    seq_t sub = (seq_t)n + (seq_t)2;

    /* a(n) = max(0, sqrtv - sub) */
    return (sqrtv > sub) ? (sqrtv - sub) : (seq_t)0;
}
