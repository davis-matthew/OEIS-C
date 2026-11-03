/*
 * A162431  Row 2 of table A162430.
 * https://oeis.org/A162431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=n*(n+1)/2 .. n*(n+1)/2+n} [x^m] S(x)^2, where S(x)
 *            = Sum_{k>=0} x^(k*(k+1)/2). The coefficient of x^m in S(x)^2
 *            equals the number of ordered pairs (i,j) with T(i)+T(j)=m,
 *            T(k)=k*(k+1)/2. This implementation counts those pairs directly
 * Range:     u8 n <= 2, u16 n <= 62, u32 n <= 254, u64 n <= 254,
 *            u128 n <= 254, big any n
 * Validated: standard, pass, 85 terms through n = 250 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
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

/* Count how many triangular numbers T(t)=t*(t+1)/2 lie in [lo,hi] (lo,hi>=0). */
static seq_t count_triangles_between(seq_t lo, seq_t hi)
{
    if (lo > hi)
        return 0;

    /* Highest t with T(t)<=hi */
    seq_t d_hi   = floor_sqrt((seq_t)1 + ((seq_t)8 * hi));
    seq_t hi_idx = (d_hi - (seq_t)1) / (seq_t)2;
    while (((hi_idx * ((hi_idx + (seq_t)1))) / (seq_t)2) > hi)
        hi_idx--;

    /* Lowest t with T(t)>=lo */
    seq_t d_lo   = floor_sqrt((seq_t)1 + ((seq_t)8 * lo));
    seq_t lo_idx = (d_lo - (seq_t)1) / (seq_t)2;
    while (((lo_idx * ((lo_idx + (seq_t)1))) / (seq_t)2) < lo)
        lo_idx++;

    if (hi_idx < lo_idx)
        return 0;

    return (hi_idx - lo_idx + (seq_t)1);
}

seq_t a(idx_t n)
{
    seq_t L = ((seq_t)n * ((seq_t)n + (seq_t)1)) / (seq_t)2;
    seq_t U = L + ((seq_t)n);
    seq_t total = 0;

    for (idx_t i = 0; ; ++i)
    {
        seq_t Ti = ((seq_t)i * ((seq_t)i + (seq_t)1)) / (seq_t)2;
        if (Ti > U)
            break;

        seq_t lo = (Ti >= L) ? (seq_t)0 : (L - Ti);
        seq_t hi = U - Ti;   /* guaranteed >= 0 */

        total += count_triangles_between(lo, hi);
    }

    return total;
}
