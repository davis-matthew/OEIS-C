/*
 * A033788  Product t2(q^d); d | 29, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd solutions to x^2 + 29*y^2 = 8*n +
 *            30
 * Range:     u8 none, u16 n <= 2031, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9767 (2026-11-01)
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
    seq_t base = 8 * (seq_t)n + 30;   /* RHS constant: 8*n + 30   */
    seq_t count = 0;                  /* number of solutions      */

    for (seq_t y = 1; ; y += 2) {     /* iterate over odd y≥1      */
        seq_t y2   = y * y;           /* y^2                       */
        seq_t term = 29 * y2;         /* 29*y^2                    */
        if (term >= base) break;      /* rhs would become ≤ 0      */
        seq_t rhs = base - term;      /* positive odd quantity     */

        seq_t s = floor_sqrt(rhs);    /* integer sqrt of rhs       */
        if (s * s == rhs && (s & 1))  /* perfect odd square?      */
            ++count;
    }

    return count;
}
