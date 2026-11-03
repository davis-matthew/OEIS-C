/*
 * A040498  Continued fraction for sqrt(521).
 * https://oeis.org/A040498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the n‑th partial quotient in the simple continued *
 *            fraction expansion of √521. * Implementation uses the standard
 *            recurrence: * * a0 = ⌊√521⌋ * m0 = 0, d0 = 1 * for k ≥ 1: * m_k
 *            = d_{k‑1} * a_{k‑1} – m_{k‑1} * d_k = (521 – m_k²) / d_{k‑1} *
 *            a_k = ⌊(a0 + m_k) / d_k⌋ * * The function recom...
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
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
    /* Integer part of √521 */
    const seq_t a0 = floor_sqrt((seq_t)521);

    /* a(0) is simply a0 */
    if (n == 0)
        return a0;

    seq_t m = 0;      /* m_{k-1} */
    seq_t d = 1;      /* d_{k-1} */
    seq_t aik = a0;   /* a_{k-1} */

    /* Iterate up to the desired index */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t m_next = d * aik - m;
        seq_t d_next = ((seq_t)521 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        if (k == n)
            return a_next;

        /* Prepare for next iteration */
        m = m_next;
        d = d_next;
        aik = a_next;
    }

    /* Unreachable, but keep compiler happy */
    return 0;
}
