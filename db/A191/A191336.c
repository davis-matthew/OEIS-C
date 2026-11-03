/*
 * A191336  (A022838 mod 2)+(A054406 mod 2).
 * https://oeis.org/A191336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ([n*sqrt(3)] mod 2)+([n*(3+sqrt(3))/2] mod 2)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Validated: partial, pass, 132 terms through n = 132 (2026-11-01)
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

/* Compute A191336 (OEIS).  Works for any seq_t and idx_t provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1, but guard against 0 */
        return 0;

    /* a_n = floor(n*sqrt(3))  computed as floor(sqrt(3*n*n)) */
    seq_t nn   = (seq_t)n;
    seq_t sq3n = nn * nn * (seq_t)3;
    seq_t a_n  = floor_sqrt(sq3n);

    /* b_n = floor(n*(3+sqrt(3))/2) = floor((3n + a_n)/2) */
    seq_t three_n = (seq_t)3 * nn;
    seq_t b_n     = (three_n + a_n) / (seq_t)2;

    /* return sum of parity bits */
    return (a_n & (seq_t)1) + (b_n & (seq_t)1);
}
