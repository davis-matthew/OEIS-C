/*
 * A040750  Continued fraction for sqrt(778).
 * https://oeis.org/A040750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(778).
 * a(0) = floor(sqrt(778)) = 27
 * For n ≥ 1, a(n) is the n‑th term (with n counting from 1) of the
 * periodic part of the continued‑fraction expansion of √778.
 * The period is generated once on the first call and cached for future calls.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute the period once */
    static seq_t period[128];
    static size_t period_len = 0;

    if (period_len == 0) {
        /* Square‑free part D = 778 */
        const seq_t D = (seq_t)778ULL;   /* cast to seq_t */
        const seq_t a0 = (seq_t)27ULL;   /* floor(sqrt(778)) */

        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a_val = a0;

        size_t len = 0;
        do {
            /* m_{k+1} = d_k * a_k - m_k */
            seq_t m_next = d * a_val - m;

            /* d_{k+1} = (D - m_{k+1}^2) / d_k */
            seq_t d_next = (D - m_next * m_next) / d;

            /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
            seq_t a_next = (a0 + m_next) / d_next;

            period[len++] = a_next;

            m = m_next;
            d = d_next;
            a_val = a_next;
        } while (a_val != (seq_t)2 * a0);   /* period ends when a_k = 2*a0 */

        period_len = len;
    }

    if (n == 0)
        return (seq_t)27ULL;

    /* index into the period: term a(1) corresponds to period[0] */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
