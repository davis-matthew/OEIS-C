/*
 * A022851  a(n) = integer nearest n*x, where x = sqrt(8).
 * https://oeis.org/A022851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n*sqrt(8)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9465 (2026-11-01)
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
    seq_t s = nn * nn * 8;          /* s = 8*n^2 */
    seq_t k = floor_sqrt(s);        /* k = floor(sqrt(s)) */
    seq_t r = s - k * k;            /* remainder s - k^2 */
    seq_t cmp = 4 * r;              /* 4*(s - k^2) */
    seq_t bound = 4 * k + 1;        /* 4*k + 1 */

    if (cmp >= bound)
        return k + 1;               /* nearest integer is k+1 */
    else
        return k;                   /* nearest integer is k */
}
