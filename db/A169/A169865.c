/*
 * A169865  The sequence T of a pair S, T generalizing Golomb's sequence
 *          A001462 and the pair A093848, A169863. See Comments for
 *          definition.
 * https://oeis.org/A169865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*n + floor((sqrt(1+4*n)-1)/2)
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
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
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t t = nn * 4 + 1;        /* compute 1 + 4*n */
    seq_t s = floor_sqrt(t);     /* floor sqrt of the above */
    seq_t cnt = (s - 1) / 2;     /* count of 3‑increments up to n */
    return nn * 2 + cnt;         /* final value of the sequence */
}
