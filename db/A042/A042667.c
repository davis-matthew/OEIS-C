/*
 * A042667  Denominators of continued fraction convergents to sqrt(863).
 * https://oeis.org/A042667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continual fraction denominators for sqrt(863)
   a(n) = Q_n where Q_0 = 1, Q_{-1} = 0 and
   Q_k = a_k * Q_{k-1} + Q_{k-2}
   using the recurrence for the continued fraction of sqrt(863). */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* First term */
    if (n == 0)
        return (seq_t)1;

    /* Constants for the continued fraction of sqrt(863) */
    const idx_t D     = (idx_t)863;
    const idx_t a0    = (idx_t)29;  /* floor(sqrt(863)) */

    /* State variables for the continued fraction algorithm */
    idx_t m       = 0;   /* m_0 */
    idx_t d       = 1;   /* d_0 */
    idx_t aPrev   = a0;  /* a_{k-1} , initially a_0 */

    /* Convergent denominators */
    seq_t qPrev2 = 0;    /* Q_{-1} */
    seq_t qPrev1 = 1;    /* Q_0 */

    seq_t qCur = 0;      /* Q_k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next continued‑fraction coefficient a_k */
        m = d * aPrev - m;
        d = (D - m * m) / d;          /* division is exact */
        aPrev = (a0 + m) / d;

        /* Compute the denominator Q_k */
        qCur = ((seq_t)aPrev) * qPrev1 + qPrev2;

        /* Shift for next iteration */
        qPrev2 = qPrev1;
        qPrev1 = qCur;
    }

    return qCur;
}
