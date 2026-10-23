/*
 * A040582  Continued fraction for sqrt(607).
 * https://oeis.org/A040582
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

/* Continued fraction for sqrt(607):
   a(0) = floor(sqrt(607)) = 24
   For n ≥ 1:
       m₀ = 0, d₀ = 1, a₀ = 24
       mₖ   = dₖ₋₁ * aₖ₋₁ - mₖ₋₁
       dₖ   = (607 - mₖ²) / dₖ₋₁
       aₖ   = (a₀ + mₖ) / dₖ
   a(n) is aₙ.                                    */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t D  = (seq_t)607;          /* 607 as seq_t */
    static const seq_t A0 = (seq_t)24;           /* floor(sqrt(607)) */

    if (n == 0)
        return A0;

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_cur = A0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_new = d * a_cur - m;
        seq_t d_new = (D - m_new * m_new) / d;
        seq_t a_next = (A0 + m_new) / d_new;
        m = m_new;
        d = d_new;
        a_cur = a_next;
    }
    return a_cur;
}
