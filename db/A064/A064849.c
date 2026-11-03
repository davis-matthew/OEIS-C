/*
 * A064849  Period of continued fraction for sqrt(3)*n.
 * https://oeis.org/A064849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the period of the continued fraction expansion
 *            * of sqrt(3) * n; equivalently of sqrt(3*n^2)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 98 terms through n = 953 (2026-11-01)
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
    /* Compute D = 3*n^2.  All arithmetic is done in seq_t as required. */
    seq_t n_seq = (seq_t)n;
    seq_t D = 3 * n_seq * n_seq;

    /* Integer part of sqrt(D).  floor_sqrt is a supplied utility. */
    seq_t a0 = floor_sqrt(D);

    /* Standard continued‑fraction algorithm for sqrt(D). */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_coeff = a0;
    seq_t period = 0;

    do {
        m = d * a_coeff - m;
        d = (D - m * m) / d;
        a_coeff = (a0 + m) / d;
        period++;
    } while (a_coeff != 2 * a0);

    return period;
}
