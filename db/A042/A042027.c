/*
 * A042027  Denominators of continued fraction convergents to sqrt(537).
 * https://oeis.org/A042027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = denominator of the n-th convergent of the continued
 * fraction for √537, computed with the standard recurrence:
 *   q_{-2} = 1, q_{-1} = 0
 *   q_k   = a_k * q_{k-1} + q_{k-2}
 * where a_0 = ⌊√537⌋ = 23 and the subsequent a_k are obtained from
 * the usual continued‑fraction machinery (m,d,a).
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of computed denominators.  */
    static seq_t *denoms     = NULL;
    static size_t denoms_cap = 0;   /* allocated size */
    static size_t denoms_len = 0;   /* highest index + 1 computed */

    /* Continued‑fraction state, shared between calls.  */
    static const idx_t D = 537;
    static const idx_t a0 = 23;     /* floor(sqrt(537)) */

    static idx_t m_cur   = 0;          /* current m */
    static idx_t d_cur   = 1;          /* current d */
    static idx_t a_cur   = a0;         /* current a */
    static seq_t q_prev1 = 0;          /* q_{k-1} */
    static seq_t q_prev2 = 1;          /* q_{k-2} */
    static bool   enabled = false;     /* has the state been initialized? */

    /* Initialise state on first call.  */
    if (!enabled) {
        m_cur   = 0;
        d_cur   = 1;
        a_cur   = a0;
        q_prev1 = 0;
        q_prev2 = 1;
        enabled = true;
    }

    /* Return the value if it is already cached.  */
    if ((idx_t)n < denoms_len) {
        return denoms[n];
    }

    /* Allocate or grow the cache as necessary.  */
    if ((idx_t)n >= denoms_cap) {
        size_t new_cap = denoms_cap ? denoms_cap * 2 : 8;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
        if (!tmp) {
            return 0;                  /* allocation failed – not expected */
        }
        denoms     = tmp;
        denoms_cap = new_cap;
    }

    /* Extend the cache up to index n.  */
    for (size_t k = denoms_len; k <= (size_t)n; ++k) {
        /* Compute q_k.  */
        seq_t qk = (seq_t)a_cur * q_prev1 + q_prev2;
        denoms[k] = qk;

        /* Shift the previous two denominators.  */
        seq_t prev_q = q_prev1;
        q_prev1 = qk;
        q_prev2 = prev_q;

        /* Compute the next continued‑fraction coefficient for future work.  */
        /* The formulas are integer, all variables are small enough for
         * 64‑bit arithmetic.  */
        idx_t m_next = d_cur * a_cur - m_cur;
        idx_t d_next = (D - m_next * m_next) / d_cur;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Update state for the next iteration.  */
        m_cur = m_next;
        d_cur = d_next;
        a_cur = a_next;
    }

    denoms_len = (size_t)n + 1;
    return denoms[n];
}
