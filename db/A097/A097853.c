/*
 * A097853  Period of continued fraction for square root of n (or 1 if n is a
 *          square).
 * https://oeis.org/A097853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = period of continued fraction of sqrt(n) (1 if n is a
 *            square)
 * Range:     u8 n <= 63, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Validated: partial, pass, 103 terms through n = 103 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    /* If n is a perfect square return 1 */
    seq_t sq = floor_sqrt((seq_t)n);
    if (sq * sq == (seq_t)n) return (seq_t)1;

    /* Standard continued‑fraction expansion for sqrt(n) */
    seq_t a0 = sq;          /* floor(sqrt(n))                     */
    seq_t m  = 0;           /* m_0 = 0                            */
    seq_t d  = 1;           /* d_0 = 1                            */
    seq_t a_val = a0;        /* a_0                               */
    seq_t period = 0;       /* count of partial quotients …       */

    for (;;)
    {
        m = d * a_val - m;                           /* m_{i+1} */
        d = ((seq_t)n - m * m) / d;                   /* d_{i+1} */
        a_val = (a0 + m) / d;                        /* a_{i+1} */
        period++;                                    /* advance period length          */
        if (a_val == (a0 << 1)) break;                /* period ends when a == 2*a0     */
    }
    return period;
}
