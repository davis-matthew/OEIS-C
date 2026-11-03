/*
 * A206906  n+[ns/r]+[nt/r], where []=floor, r=1/3, s=sqrt(3), t=1/s.
 * https://oeis.org/A206906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊3 n √3⌋ + ⌊9 n √3⌋ = n + ⌊√(3 n²)⌋ + ⌊√(27 n²)⌋
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values are computed using integer arithmetic only, with the provided
   floor_sqrt routine for the square‑root operations. */
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
    if (n == 0)   /* undefined for n=0, but return 0 for safety */
        return (seq_t)0;

    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;          /* n^2 */
    seq_t t1   = sq * (seq_t)3;    /* 3 * n^2 */
    seq_t t2   = sq * (seq_t)27;   /* 27 * n^2 */

    seq_t r1   = floor_sqrt(t1);   /* ⌊√(3 n²)⌋ */
    seq_t r2   = floor_sqrt(t2);   /* ⌊√(27 n²)⌋ */

    return nn + r1 + r2;
}
