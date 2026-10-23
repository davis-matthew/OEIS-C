/*
 * A040667  Continued fraction for sqrt(694).
 * https://oeis.org/A040667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A040667: Continued fraction coefficients of √694.
 * Each term a(n) for n ≥ 0 is given by the standard
 * recurrence for the continued fraction expansion of a square‑free
 * irrational:
 *
 *   a0      = ⌊√694⌋
 *   m0      = 0
 *   d0      = 1
 *   a0      = a0
 *   For k ≥ 0
 *       m(k+1) = d(k) * a(k) – m(k)
 *       d(k+1) = (694 – m(k+1)²) / d(k)
 *       a(k+1) = ⌊ (a0 + m(k+1)) / d(k+1) ⌋
 *
 * This function computes a(n) directly in O(n) time without
 * caching, which satisfies the required performance for n ≤ 10 000.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D = (seq_t)694;          /* radicand */
    seq_t a0      = floor_sqrt(D);       /* ⌊√694⌋   */

    /* values for the recurrence */
    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a_val = a0;

    if (n == 0)
        return a0;

    /* iterate n times to reach a(n) */
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t m_next = d * a_val - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* prepare for next step */
        m     = m_next;
        d     = d_next;
        a_val = a_next;
    }

    return a_val;
}
