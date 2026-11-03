/*
 * A040949  Continued fraction for sqrt(981).
 * https://oeis.org/A040949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(981).  a(n) is the n‑th partial quotient
   in the continued fraction expansion of √981.  The algorithm follows
   the standard recurrence:
       m₀ = 0, d₀ = 1, a₀ = ⌊√N⌋;
       mᵢ₊₁ = dᵢ·aᵢ – mᵢ;
       dᵢ₊₁ = (N – mᵢ₊₁²) / dᵢ;
       aᵢ₊₁ = ⌊(a₀ + mᵢ₊₁) / dᵢ₊₁⌋.
   Here N = 981.  This computes a(n) in O(n) time.  */

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
    /* The radicand */
    const seq_t N = (seq_t)981;
    /* Base term a₀ */
    const seq_t a0 = floor_sqrt(N);

    if (n == 0)
        return a0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_curr = a0;
    idx_t i;

    for (i = 1; i <= n; ++i) {
        /* Compute next state */
        const seq_t m_next = d * a_curr - m;
        const seq_t d_next = (N - m_next * m_next) / d;
        const seq_t a_next = (a0 + m_next) / d_next;

        if (i == n)
            return a_next;

        /* Prepare for next iteration */
        m = m_next;
        d = d_next;
        a_curr = a_next;
    }

    /* Should never reach here */
    return 0;
}
