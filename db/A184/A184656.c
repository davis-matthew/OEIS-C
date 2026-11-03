/*
 * A184656  floor(nr+h), where r=(1+sqrt(5))/2, h=-1/2; complement of A184657.
 * https://oeis.org/A184656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor((n-1 + n*sqrt(5)) / 2)
 *           equivalently a(n) = floor(n*phi - 1/2),  phi = (1+sqrt(5))/2
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
    /* n ≥ 1 (offset 1).  Behaviour for n=0 is undefined but safe. */
    seq_t nn = (seq_t)n;

    /* Compute floor(sqrt(5*n*n)) exactly using integer arithmetic. */
    seq_t prod = nn * nn * (seq_t)5;
    seq_t s = floor_sqrt(prod);

    /* Return floor((n-1 + s) / 2).  All operations are integer. */
    return ((nn - (seq_t)1) + s) / (seq_t)2;
}
