/*
 * A040352  Continued fraction for sqrt(372).
 * https://oeis.org/A040352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The terms are the partial quotients of the continued fraction of sqrt(372).
   a(0) = 19, a(1..6) = 3,2,12,2,3,38, and thereafter the period repeats. */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction.  The period length is 6. */
    #define PERIOD_LEN 6
    static seq_t period[PERIOD_LEN];
    static int initialized = 0;

    if (!initialized) {
        const seq_t N   = (seq_t)372;
        const seq_t a0  = (seq_t)19;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        size_t pos = 0;

        /* Generate the periodic part until the first d_next == 1. */
        while (1) {
            seq_t m_next  = d * a - m;
            seq_t d_next  = (N - m_next * m_next) / d;
            seq_t a_next  = (a0 + m_next) / d_next;

            period[pos++] = a_next;      /* store the term of the period */
            if (d_next == 1) break;      /* last term of the period found */

            m = m_next;
            d = d_next;
            a = a_next;
        }
        initialized = 1;
    }

    if (n == 0) return (seq_t)19;   /* the non‑periodic initial term */

    /* For n >= 1, the term is the (n‑1)‑th element of the period. */
    size_t idx = (size_t)(n - 1) % PERIOD_LEN;
    return period[idx];
}
