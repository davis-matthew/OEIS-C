/*
 * A040790  Continued fraction for sqrt(819).
 * https://oeis.org/A040790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = floor(sqrt(819)) = 28. For n ≥ 1, a(n) equals the nth
 *            term of the periodic continued fraction of √819. The period is
 *            computed once on the first call and then reused. The period ends
 *            when the term 2·a(0) = 56 appears for the first time
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for √819 (once). */
    static bool initialized = false;
    static seq_t period[128];      /* sufficiently large for the period */
    static idx_t period_len = 0;

    const seq_t N   = (seq_t)819;   /* the number under the square root   */
    const seq_t a0  = (seq_t)28;    /* floor(sqrt(N))                     */

    if (!initialized) {
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a_curr = a0;
        size_t idx = 0;

        /* Compute terms until the first occurrence of 2·a0 (=56). */
        while (idx < 128) {
            seq_t m_next = d * a_curr - m;
            seq_t d_next = (N - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[idx++] = a_next;

            if (a_next == (seq_t)(2 * a0))
                break;

            m       = m_next;
            d       = d_next;
            a_curr  = a_next;
        }

        period_len = (idx_t)idx;
        initialized = true;
    }

    /* a(0) is a special first term. */
    if (n == 0)
        return a0;

    /* For n ≥ 1, return the term in the periodic block. */
    if (period_len == 0) /* fallback: should never happen */
        return (n % (idx_t)10 == 0 ? (seq_t)56 : (seq_t)1);

    idx_t mod = (n - (idx_t)1) % period_len;
    return period[mod];
}
