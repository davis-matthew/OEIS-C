/*
 * A135708  Minimal total number of edges in a polyhex consisting of n
 *          hexagonal cells.
 * https://oeis.org/A135708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n + ceil(sqrt(12*n - 3))
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 983 (2026-11-01)
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
    seq_t tn = (seq_t)n;                 /* cast index to seq_t */
    seq_t s  = (seq_t)12 * tn - (seq_t)3;/* 12*n - 3 */
    seq_t r  = floor_sqrt(s);            /* floor(sqrt(s)) */
    if (r * r < s) {                    /* if not a perfect square */
        r = r + (seq_t)1;                /* ceil(sqrt(s)) */
    }
    return (seq_t)3 * tn + r;            /* 3*n + ceil(sqrt(12*n-3)) */
}
