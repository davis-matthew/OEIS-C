/*
 * A042357  Denominators of continued fraction convergents to sqrt(705).
 * https://oeis.org/A042357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1; a(n) is the denominator of the n‑th * convergent of
 *            the continued fraction for √705. * Computed via the standard
 *            recurrence for convergents: * p_k = a_k p_{k-1} + p_{k-2} * q_k
 *            = a_k q_{k-1} + q_{k-2} * with a_0 = ⌊√705⌋ = 26 and the usual
 *            recurrence for * a_k, m_k, d_k
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 19, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent state between calls */
    static seq_t *denoms = NULL;          /* cached denominators a(0)…a(last) */
    static idx_t denoms_len = 0;          /* number of cached values */

    /* Continued fraction constants and running state */
    static const seq_t a0 = 26;           /* ⌊√705⌋ */
    static seq_t p_prev2 = 0, p_prev1 = 1; /* p_{k-2}, p_{k-1} */
    static seq_t q_prev2 = 1, q_prev1 = 0; /* q_{k-2}, q_{k-1} */
    static seq_t m = 0, d = 1;           /* m_k, d_k values */

    /* Initialise on first call */
    if (denoms == NULL)
    {
        denoms_len = 1;                                 /* a(0) stored */
        denoms = (seq_t *)malloc(sizeof(seq_t));
        if (denoms == NULL)
            return 0;  /* allocation failure – unreachable in test harness */

        denoms[0] = 1;                                 /* a(0) = 1 */

        /* compute convergent 0 (p0/q0) */
        seq_t p0 = a0 * p_prev1 + p_prev2;             /* 26*1 + 0 = 26   */
        seq_t q0 = a0 * q_prev1 + q_prev2;             /* 26*0 + 1 = 1    */

        /* shift state for next iteration (k = 1) */
        p_prev2 = p_prev1;
        p_prev1 = p0;
        q_prev2 = q_prev1;
        q_prev1 = q0;

        /* update m, d for k = 1 */
        m = d * a0 - m;                                /* 1*26 - 0 = 26  */
        d = (seq_t)(705 - m * m) / d;                  /* (705-26²)/1 = 29 */
    }

    /* Compute up to the requested index if necessary */
    while (denoms_len <= n)
    {
        /* a_k = ⌊(a0 + m_k) / d_k⌋ */
        seq_t a_k = (a0 + m) / d;

        /* m_{k+1} and d_{k+1} for the next step */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (seq_t)(705 - m_next * m_next) / d;

        /* Next convergent numerator/denominator */
        seq_t p_next = a_k * p_prev1 + p_prev2;
        seq_t q_next = a_k * q_prev1 + q_prev2;

        /* Append denominator to cache */
        seq_t *tmp = (seq_t *)realloc(denoms, (denoms_len + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return 0;  /* allocation failure – unreachable in test harness */
        denoms = tmp;
        denoms[denoms_len] = q_next;
        denoms_len++;

        /* Advance state for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_next;
        q_prev2 = q_prev1;
        q_prev1 = q_next;
        m = m_next;
        d = d_next;
    }

    return denoms[n];
}
