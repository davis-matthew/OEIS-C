/*
 * A040496  Continued fraction for sqrt(519).
 * https://oeis.org/A040496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A040496 – Continued fraction for sqrt(519)
 *
 *  a(0) = 22
 *  For n>0, a(n) is the&nbsp;(n‑1)st element of the periodic
 *  part of the continued‑fraction expansion of sqrt(519).
 *  The period stops when the partial quotient equals 2*a(0) = 44.
 *
 *  The algorithm is the standard recurrence for quadratic irrationals:
 *      m_{k+1} = d_k * a_k – m_k
 *      d_{k+1} = (D – m_{k+1}^2) / d_k
 *      a_{k+1} = ⌊(a_0 + m_{k+1}) / d_{k+1}⌋
 *  where D = 519 and a_0 = ⌊√D⌋ = 22.
 *  The period is generated once on the first call, stored in a static
 *  array, and reused for any subsequent indices.
 *
 *  The implementation is O(1) per call after the initial period
 *  construction and works for any seq_t and idx_t types
 *  that satisfy the constraints described in the problem statement.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for the continued fraction */
    static bool initialized = false;
    static seq_t period[256];
    static idx_t period_len = 0;
    static const seq_t a0 = (seq_t)22;   /* floor(sqrt(519)) */

    if (!initialized) {
        /* generate the periodic part of the continued fraction of sqrt(519) */
        uint64_t m = 0;
        uint64_t d = 1;
        seq_t   a_k = a0;
        idx_t   i   = 0;

        do {
            uint64_t m_next = d * (uint64_t)a_k - m;
            uint64_t d_next = (519ULL - m_next * m_next) / d;
            seq_t   a_next = (a0 + (seq_t)m_next) / (seq_t)d_next;

            period[i++] = a_next;
            a_k = a_next;
            m   = m_next;
            d   = d_next;
        } while (a_k != 2 * a0);

        period_len = i;
        initialized = true;
    }

    if (n == 0)
        return a0;

    /* periodic part starts at a(1) */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
