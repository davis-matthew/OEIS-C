/*
 * A172473  a(n) = floor(sqrt(2*n^5)).
 * https://oeis.org/A172473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(2*n^5))
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 978 (2026-11-01)
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
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;            /* n^2 */
    seq_t n3  = n2 * sn;            /* n^3 */
    seq_t n5  = n3 * n2;            /* n^5 = n^3 * n^2 */
    seq_t val = n5 * ((seq_t)2);    /* 2*n^5 */
    return floor_sqrt(val);         /* floor of sqrt(2*n^5) */
}
