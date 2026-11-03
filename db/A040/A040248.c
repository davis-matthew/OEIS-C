/*
 * A040248  Continued fraction for sqrt(265).
 * https://oeis.org/A040248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) follows the continued‑fraction expansion of √265
 * Range:     u8 none, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 88 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the standard recurrence:    */
/*   m0 = 0, d0 = 1, a0 = ⌊√265⌋  */
/*   m_k = d_{k-1}·a_{k-1} – m_{k-1}  */
/*   d_k = (265 – m_k²) / d_{k-1}      */
/*   a_k = ⌊(a0 + m_k) / d_k⌋          */

#include <stdint.h>
#include <stddef.h>
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
    const seq_t D = 265;                      /* the square‑free part */
    seq_t a0 = floor_sqrt(D);                /* ⌊√265⌋ = 16 */

    if (n == 0)
        return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }
    return a;
}
