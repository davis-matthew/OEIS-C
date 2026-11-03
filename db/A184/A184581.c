/*
 * A184581  a(n) = floor((n + 1/4)*(2 + sqrt(2))).
 * https://oeis.org/A184581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n + 1/4)*(2 + sqrt(2)))
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 5372, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9238 (2026-11-01)
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
    if (n == 0) {
        return (seq_t)0;                /* offset is 1; undefined for n = 0 */
    }

    /* k = 4*n + 1  (always odd) */
    seq_t k = ((seq_t)n << 2) + (seq_t)1;

    /* s = floor(k * sqrt(2))  computed as floor_sqrt(2*k*k) */
    seq_t s = floor_sqrt(((seq_t)2) * k * k);

    /* floor(k * sqrt(2) / 4) + indicator((s mod 4) >= 2) */
    seq_t q   = s >> 2;             /* s / 4 with integer division */
    seq_t r   = s & 3;              /* s % 4 (0..3) */
    seq_t add = (r >= 2) ? (seq_t)1 : (seq_t)0;

    /* 2*n  (offset part) */
    seq_t two_n = ((seq_t)n << 1);

    /* Final result */
    return two_n + q + add;
}
