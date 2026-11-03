/*
 * A205633  Expansion of f(x^3, x^7) in powers of x where f() is Ramanujan's
 *          two-variable theta function.
 * https://oeis.org/A205633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if 1 + 5*n is a perfect square, 0 otherwise
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 137 terms through n = 943 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    /* 1 + 5*n as seq_t */
    seq_t val = (seq_t)1 + (seq_t)5 * (seq_t)n;
    /* integer square root */
    seq_t root = floor_sqrt(val);
    /* check perfect square */
    return (root * root == val) ? (seq_t)1 : (seq_t)0;
}
