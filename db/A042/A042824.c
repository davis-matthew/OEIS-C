/*
 * A042824  Numerators of continued fraction convergents to sqrt(943).
 * https://oeis.org/A042824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(943). */
/* Algorithm: compute the continued‑fraction terms of sqrt(943) and
   generate the numerator p[n] of the n‑th convergent by the Pell
   recurrence: p[k] = a[k]*p[k-1] + p[k-2]. The first term a[0] = ⌊√943⌋ = 30. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The value of ⌊√943⌋ is 30. We hard‑code it to avoid floating point. */
    const uint64_t D = 943;
    const uint64_t a0 = 30;

    if (n == 0)
        return (seq_t)a0;

    /* Continued‑fraction generation for sqrt(D). */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    /* Convergent numerators: p[-1] = 1, p[0] = a0. */
    seq_t p_prev = (seq_t)1;
    seq_t p_curr = (seq_t)a0;

    /* Iterate until the n‑th convergent. */
    idx_t k;
    for (k = 1; k <= n; ++k)
    {
        /* Compute the next term a_k of the continued fraction. */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        /* Update the convergent numerator via the recurrence. */
        seq_t p_next = (seq_t)a * p_curr + p_prev;
        p_prev = p_curr;
        p_curr = p_next;
    }

    return p_curr;
}
