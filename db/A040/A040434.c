/*
 * A040434  Continued fraction for sqrt(456).
 * https://oeis.org/A040434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(456):
 * a(0) = 21;
 * for n ≥ 1:
 *   mₖ₊₁ = dₖ * aₖ − mₖ
 *   dₖ₊₁ = (456 − mₖ₊₁²) / dₖ
 *   aₖ₊₁ = ⌊ (21 + mₖ₊₁) / dₖ₊₁ ⌋
 * This produces the repeating pattern 21, 2, 1, 4, 1, 2, 42, ….
 */
#include <stddef.h>       /* for size_t, not strictly required but harmless */

seq_t a(idx_t n)
{
    const seq_t a0 = 21;    /* floor(sqrt(456)) */
    const seq_t S  = 456;   /* the non‑square under the root */

    if (n == 0)
        return a0;

    seq_t m = 0;            /* m₀ */
    seq_t d = 1;            /* d₀ */
    seq_t a_prev = a0;      /* a₀ */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t m_next = d * a_prev - m;
        seq_t d_next = (S - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_prev = a_next;
    }

    return a_prev;
}
