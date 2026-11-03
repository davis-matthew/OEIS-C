/*
 * A137353  Minimal number of edges in a (P_4,n)-edge stable graph.
 * https://oeis.org/A137353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ceil( (sqrt(8*n + 9) + 3) / 2 ), for n >= 2. a(1) = 4
 * Range:     u8 n <= 6, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
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
    if (n == 0) return (seq_t)0;          /* undefined offset; return 0 safety */
    if (n == 1) return (seq_t)4;          /* base term */
    seq_t x = ((seq_t)8 * (seq_t)n) + (seq_t)9;
    seq_t t = floor_sqrt(x);              /* integer floor of sqrt(8*n+9) */
    seq_t ceil_term;
    if (t * t == x)                      /* 8*n+9 is a perfect square */
        ceil_term = (t + (seq_t)4) / (seq_t)2;
    else                                  /* otherwise */
        ceil_term = (t + (seq_t)5) / (seq_t)2;
    return (seq_t)n + ceil_term;
}
