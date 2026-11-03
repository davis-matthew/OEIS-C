/*
 * A080757  First differences of Beatty sequence A022838(n) = floor(n
 *          sqrt(3)).
 * https://oeis.org/A080757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor_sqrt(3 * (n+1)^2) - floor_sqrt(3 * n^2)
 * Range:     u8 n <= 3, u16 n <= 72, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 106 terms through n = 105 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from a(n) = floor((n+1)*sqrt(3)) - floor(n*sqrt(3))   */
/* using floor(n*sqrt(3)) = floor_sqrt(3*n^2) */

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
    /* cast idx_t to seq_t for arithmetic */
    seq_t n_sq   = (seq_t)n * (seq_t)n;        /* n^2            */
    seq_t n1_sq  = (seq_t)(n + 1) * (seq_t)(n + 1); /* (n+1)^2       */

    /* compute floor of sqrt of 3 * n^2 and 3 * (n+1)^2 */
    seq_t sqrt_n   = floor_sqrt((seq_t)3 * n_sq);
    seq_t sqrt_n1  = floor_sqrt((seq_t)3 * n1_sq);

    return sqrt_n1 - sqrt_n;
}
