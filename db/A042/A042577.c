/*
 * A042577  Denominators of continued fraction convergents to sqrt(817).
 * https://oeis.org/A042577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   sequence a(n) are denominators of the continued‑fraction
 *            convergents to sqrt(817)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(817) */
    const idx_t D = 817;          /* D = 817 */
    const idx_t S = 28;           /* S = floor(sqrt(817)) */

    /* Static state: memoised denominators and continued‑fraction data */
    static seq_t *q = NULL;       /* array of Q_k values (k >= 0)   */
    static size_t q_cap = 0;      /* current capacity of q array   */
    static size_t q_len = 0;      /* number of values stored       */
    static idx_t m = 0;           /* m for continued­fraction step */
    static idx_t d = 1;           /* d for continued­fraction step */
    static idx_t a_cur = S;       /* a_n for current n             */
    static seq_t q_prev1 = 1;     /* Q_{k-1}                       */
    static seq_t q_prev2 = 0;     /* Q_{k-2}                       */
    static idx_t last = 0;        /* last computed index           */

    /* Initialise on first call */
    if (q == NULL) {
        q_cap = 16;
        q = (seq_t *)malloc(q_cap * sizeof(seq_t));
        if (!q) exit(EXIT_FAILURE);           /* allocation failure */
        q[0] = (seq_t)1;                     /* Q_0 = 1                 */
        q_len = 1;
        /* a_0 is already S (28) but Q_0 does not depend on it. */
        m = 0;
        d = 1;
        a_cur = S;
        q_prev1 = (seq_t)1;  /* Q_0 */
        q_prev2 = 0;         /* Q_{-1} */
        last = 0;
    }

    /* Extend memoisation up to requested index n */
    while (last < n) {
        /* Generate next continued‑fraction term a_{next} */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (S + m_next) / d_next;

        /* Compute next denominator Q_{next} = a_next * Q_{k-1} + Q_{k-2} */
        seq_t new_q = a_next * q_prev1 + q_prev2;

        /* Ensure array capacity */
        if (q_len == q_cap) {
            q_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(q, q_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
            q = tmp;
        }
        q[q_len++] = new_q;

        /* Update state for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = new_q;
        m = m_next;
        d = d_next;
        a_cur = a_next;
        ++last;
    }

    return q[n];
}
