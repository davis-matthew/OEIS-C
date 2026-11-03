/*
 * A229118  Distance from the n-th triangular number to the nearest square.
 * https://oeis.org/A229118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = distance between the nth triangular number T_n =
 *            n*(n+1)/2 * and the nearest perfect square. Concretely * a(n) =
 *            min( |T_n – m²| , |(m+1)² – T_n| ) where m = floor(sqrt(T_n))
 * Range:     u8 n <= 10, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 106 terms through n = 861 (2026-11-01)
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
    if (n == 0) return 0;               /* offset 1; 0 is not defined */
    seq_t N = (seq_t)n;
    seq_t T = N * (N + 1) / 2;          /* nth triangular number */

    seq_t m = floor_sqrt(T);            /* floor(sqrt(T))      */
    seq_t sq = m * m;                   /* m²                  */
    seq_t diff1 = T - sq;               /* T – m²   (non‑negative) */

    seq_t mp1 = m + 1;                  /* m+1                 */
    seq_t sq2 = mp1 * mp1;              /* (m+1)²              */
    seq_t diff2 = sq2 - T;              /* (m+1)² – T  (non‑negative) */

    return (diff1 < diff2) ? diff1 : diff2;
}
