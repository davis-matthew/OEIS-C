/*
 * A042361  Denominators of continued fraction convergents to sqrt(707).
 * https://oeis.org/A042361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of the continued
 *            fraction of √707
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static state */
    static idx_t current_max = 0;       /* highest index computed */
    static size_t cap = 0;              /* capacity of denoms array */
    static seq_t *denoms = NULL;        /* cache of denominators */

    /* recurrence state for convergents */
    static seq_t p_minus2 = (seq_t)0, p_minus1 = (seq_t)1;
    static seq_t q_minus2 = (seq_t)1, q_minus1 = (seq_t)0;

    /* state for computing partial quotients of √707 */
    static seq_t m = (seq_t)0, d = (seq_t)1;
    static const seq_t a0 = (seq_t)26;          /* floor(sqrt(707))   */
    static const seq_t D  = (seq_t)707;          /* D = 707            */

    /* lazy initialization */
    if (denoms == NULL) {
        cap = 1;
        denoms = (seq_t*)malloc(cap * sizeof(seq_t));

        /* first convergent (index 0) */
        seq_t a_cur = a0;
        seq_t p_cur = a_cur * p_minus1 + p_minus2;
        seq_t q_cur = a_cur * q_minus1 + q_minus2;
        denoms[0] = q_cur;

        /* shift for next step */
        p_minus2 = p_minus1;   p_minus1 = p_cur;
        q_minus2 = q_minus1;   q_minus1 = q_cur;

        /* compute state for the next partial quotient */
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (D - m_next * m_next) / d;
        m = m_next;           d = d_next;

        current_max = 0;
    }

    /* grow cache if necessary and compute up to n */
    if (n > current_max) {
        size_t needed = (size_t)n + 1;
        if (needed > cap) {
            size_t new_cap = cap ? cap << 1 : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(denoms, new_cap * sizeof(seq_t));
            denoms = tmp;
            cap = new_cap;
        }
        for (idx_t k = current_max + 1; k <= n; ++k) {
            /* next partial quotient */
            seq_t a_cur = (a0 + m) / d;          /* floor division */

            /* new convergent */
            seq_t p_cur = a_cur * p_minus1 + p_minus2;
            seq_t q_cur = a_cur * q_minus1 + q_minus2;
            denoms[k] = q_cur;

            /* shift p/q for next iteration */
            p_minus2 = p_minus1;   p_minus1 = p_cur;
            q_minus2 = q_minus1;   q_minus1 = q_cur;

            /* update state for following partial quotient */
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (D - m_next * m_next) / d;
            m = m_next;           d = d_next;
        }
        current_max = n;
    }

    return denoms[n];
}
