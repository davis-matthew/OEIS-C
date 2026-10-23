/*
 * A040403  Continued fraction for sqrt(424).
 * https://oeis.org/A040403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040403: terms of the simple continued fraction for sqrt(424) */
/* a(0) = floor(sqrt(424)) = 20.  Subsequent terms are obtained by the
 * standard recurrence:
 *   m_{k+1} = d_k * a_k - m_k
 *   d_{k+1} = (N - m_{k+1}^2) / d_k
 *   a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})
 * with N = 424 and a0 = floor(sqrt(N)) = 20.  This computes the nth
 * coefficient a(n) in O(n) time.  All intermediate arithmetic is
 * performed in uint64_t so that the function works for any seq_t
 * bounded reasonably (including 8‑bit types). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(424) */
    const uint64_t N = 424ULL;   /* 424 = 4 * 106 = 4 * 2 * 53 */
    const uint64_t a0 = 20ULL;   /* floor(sqrt(424)) */

    uint64_t m = 0ULL;
    uint64_t d = 1ULL;
    uint64_t a_cur = a0;

    if (n == 0U) {
        return (seq_t)a_cur;
    }

    /* iterate to compute a(n) */
    for (idx_t i = 1U; i <= n; ++i) {
        uint64_t m_next = d * a_cur - m;
        uint64_t d_next = (N - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return (seq_t)a_cur;
}
