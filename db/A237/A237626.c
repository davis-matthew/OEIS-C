/*
 * A237626  Sum of a^2 + b^2 for all nonnegative integers a,b such that
 *          b^2-a^2 = 4n.
 * https://oeis.org/A237626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|4n, d≤d', same parity} (d²+d'²)/2, where d·d' = 4n,
 *            so that a = (d'−d)/2, b = (d'+d)/2
 * Range:     u8 n <= 7, u16 n <= 154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9468 (2026-11-01)
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
    seq_t val   = 0;
    seq_t fourn = (seq_t)n * 4;                /* 4n as seq_t   */

    /* integer square‑root of 4n (fast path for all builds)  */
    seq_t sqrt_flo = floor_sqrt(fourn);        /* floor(sqrt(4n)) */
    idx_t limit = (idx_t)sqrt_flo;             /* safe cast, sqrt(4n) ≤ 2³³ for n<2⁶⁴ */

    for (idx_t d = 1; d <= limit; ++d)
    {
        seq_t sd  = (seq_t)d;                  /* divisor as seq_t */

        /* check if d divides 4n */
        if ((fourn % sd) != 0)
            continue;

        seq_t dprime = fourn / sd;             /* complementary divisor */

        /* both must have same parity for integer a,b          */
        if (((sd ^ dprime) & 1) != 0)
            continue;

        seq_t sumSq = sd * sd + dprime * dprime; /* d² + d'²  */
        val += sumSq / 2;                       /* (d²+d'²)/2 */
    }

    return val;
}
