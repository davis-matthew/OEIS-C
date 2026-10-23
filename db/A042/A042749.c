/*
 * A042749  Denominators of continued fraction convergents to sqrt(905).
 * https://oeis.org/A042749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1
   a(1)=12
   a(n)= (12 if n odd else 60)*a(n-1)+a(n-2)
   (derived from the continued‑fraction convergents of sqrt(905)). */
seq_t a(idx_t n)
{
    /* handle the trivial case */
    if (n == 0)
        return (seq_t)1;

    /* q_{-2} = 1, q_{-1} = 0  (denominator recurrence) */
    seq_t qprev2 = (seq_t)1;   /* q_{i-2} */
    seq_t qprev1 = (seq_t)0;   /* q_{i-1} */
    seq_t qcurr;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0)
            ai = (seq_t)30;           /* a0 = floor(sqrt(905)) */
        else if ((i & 1) == 1)
            ai = (seq_t)12;           /* odd indices */
        else
            ai = (seq_t)60;           /* even indices >0 */

        qcurr = ai * qprev1 + qprev2;   /* q_i = a_i * q_{i-1} + q_{i-2} */

        if (i == n)
            return qcurr;             /* answer for the requested n */

        /* shift for next iteration */
        qprev2 = qprev1;
        qprev1 = qcurr;
    }

    /* unreachable, but avoids compiler warnings */
    return (seq_t)0;
}
