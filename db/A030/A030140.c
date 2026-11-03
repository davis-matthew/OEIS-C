/*
 * A030140  The nonsquares squared.
 * https://oeis.org/A030140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000037(n)^2 - square of the nth nonsquare integer
 * Range:     u8 n <= 12, u16 n <= 240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9619 (2026-11-01)
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
    /* Find the nth nonsquare integer f.  The count of nonsquares <= x is
       x - ⌊√x⌋.  We search for the smallest f such that f - ⌊√f⌋ == n. */
    seq_t low  = (seq_t)n;                 /* f cannot be smaller than n */
    seq_t high = low + floor_sqrt(low) + 10; /* generous upper bound */
    seq_t n_seq = (seq_t)n;

    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        seq_t s   = floor_sqrt(mid);       /* ⌊√mid⌋ */
        seq_t diff = mid - s;              /* count of nonsquares <= mid */

        if (diff >= n_seq)
            high = mid;
        else
            low = mid + 1;
    }

    /* Now low is the nth nonsquare integer; return its square. */
    return low * low;
}
