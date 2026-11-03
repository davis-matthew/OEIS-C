/*
 * A033790  Product t2(q^d); d | 31, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd solutions (x,y) of * x^2 + 31*y^2
 *            = 8*(n+4)
 * Range:     u8 n <= 7, u16 n <= 1991, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9000 (2026-11-01)
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
    seq_t count = 0;
    /* RHS of the Diophantine equation */
    seq_t s = 8 * ((seq_t)n + (seq_t)4);

    /* maximum odd y that can appear: floor_sqrt((s-1)/31) */
    seq_t y_max = floor_sqrt((s - 1) / (seq_t)31);

    for (seq_t y = 1; y <= y_max; y += 2) {   /* iterate only odd y */
        seq_t y2 = y * y;
        seq_t term = (seq_t)31 * y2;          /* 31*y^2 */
        seq_t rem = s - term;                 /* x^2 = rem */
        seq_t x = floor_sqrt(rem);
        if (x * x == rem && (x & (seq_t)1)) { /* x odd */
            count++;
        }
    }
    return count;
}
