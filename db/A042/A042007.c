/*
 * A042007  Denominators of continued fraction convergents to sqrt(526).
 * https://oeis.org/A042007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents to sqrt(526).
   Recurrence: a(0)=1, a(1)=1; for n≥2:
   a(n)=A[n]*a(n-1)+a(n-2) where A[n] is the nth partial quotient of
   sqrt(526) with a(0)=⌊√526⌋=22. */

#include <stdlib.h>

/* Compute the exact denominator of the n-th convergent of sqrt(526). */
seq_t a(idx_t n)
{
    /* Precompute one period of the continued fraction of sqrt(526). */
    static int initialized = 0;
    static unsigned int period[128];
    static size_t period_len = 0;

    if (!initialized) {
        const int D = 526;
        const unsigned int a0 = 22;   /* floor(sqrt(526)) */

        unsigned long long m = 0, d = 1, a = a0;
        size_t len = 0;

        while (1) {
            unsigned long long m_next = d * a - m;
            unsigned long long d_next = (D - m_next * m_next) / d;
            unsigned long long a_next = (a0 + m_next) / d_next;

            period[len++] = (unsigned int)a_next;
            if (d_next == 1)            /* last coefficient is 2*a0 = 44 */
                break;
            m = m_next;
            d = d_next;
            a = a_next;
        }
        period_len = len;
        initialized = 1;
    }

    /* Compute the convergent denominator up to index n. */
    seq_t qprev2 = 1;      /* q[-2] */
    seq_t qprev1 = 0;      /* q[-1] */
    seq_t qk = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0)
            ak = (seq_t)22;                     /* a0 */
        else {
            size_t idx = (k - 1) % period_len;   /* periodic part (a1..aPeriod) */
            ak = (seq_t)period[idx];
        }
        qk = ak * qprev1 + qprev2;
        qprev2 = qprev1;
        qprev1 = qk;
    }

    return qk;
}
