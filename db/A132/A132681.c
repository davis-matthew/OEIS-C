/*
 * A132681  Infinitesimal generator matrix for a diagonally-shifted Pascal
 *          matrix, binomial(n+m,k+m), for m=1, related to Laguerre(n,x,m).
 * https://oeis.org/A132681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (√(8n+17) - 1)/2 if 8n+17 is a perfect square (√ odd), 0
 *            otherwise (OEIS A132681)
 * Range:     u8 n <= 5, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-01)
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
    // 8*n + 17
    seq_t D = ((seq_t)8 * n) + 17;
    seq_t s = floor_sqrt(D);
    if (s * s != D)          // not a perfect square
        return 0;
    if ((s & 1) == 0)        // square root must be odd
        return 0;
    return (s - 1) / 2;      // value at the special index
}
