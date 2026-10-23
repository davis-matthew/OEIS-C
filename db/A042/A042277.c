/*
 * A042277  Denominators of continued fraction convergents to sqrt(664).
 * https://oeis.org/A042277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the denominators of the convergents of the continued fraction of √664.
   The algorithm follows the standard recurrence for continued fractions of
   quadratic irrationals.  For each index n it returns q_n where q_n/q_{n-1}
   approximates √664.  Denominators are stored in a static array so that
   subsequent calls are O(1) after the first expansion up to the maximum
   requested index.  The implementation uses only integer arithmetic. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *den = NULL;                /* stored denominators q_n  */
    static size_t den_cap = 0;               /* allocated capacity */
    static idx_t den_len = 0;                /* number of values computed */

    /* state for the next iteration of the continued fraction */
    static seq_t p_prev2 = 0;                /* p_{k-2}  (initial 0)   */
    static seq_t p_prev1 = 1;                /* p_{k-1}  (initial 1)   */
    static seq_t q_prev2 = 1;                /* q_{k-2}  (initial 1)   */
    static seq_t q_prev1 = 0;                /* q_{k-1}  (initial 0)   */

    static seq_t m = 0;                      /* m_k  (initial 0)       */
    static seq_t d = 1;                      /* d_k  (initial 1)       */
    static seq_t a0 = 0;                     /* a0 = floor(√664)       */
    static seq_t a = 0;                      /* a_k for current step   */
    static int initialized = 0;              /* flag if first call   */

    const seq_t D = 664;                     /* the integer under the root */

    /* initialise the static data on the first call */
    if (!initialized) {
        a0 = floor_sqrt(D);
        a  = a0;                            /* a_0 for the first step */
        /* allocate initial capacity */
        den_cap = 16;
        den = (seq_t*)malloc(den_cap * sizeof(seq_t));
        if (!den) abort();                  /* out of memory */
        den_len = 0;
        initialized = 1;
    }

    /* extend memoised sequence up to index n */
    if (n >= den_len) {
        for (idx_t k = den_len; k <= n; ++k) {
            /* compute p_k and q_k from previous two values */
            seq_t p_cur = a * p_prev1 + p_prev2;
            seq_t q_cur = a * q_prev1 + q_prev2;

            /* store the denominator */
            if (den_len >= den_cap) {
                size_t new_cap = den_cap ? den_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(den, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                den = tmp; den_cap = new_cap;
            }
            den[den_len] = q_cur;            /* q_k becomes den[k] */
            ++den_len;

            /* shift recurrence variables for next step */
            p_prev2 = p_prev1;  p_prev1 = p_cur;
            q_prev2 = q_prev1;  q_prev1 = q_cur;

            /* compute next m, d, a for continued fraction */
            seq_t m_next = d * a - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;
            m = m_next; d = d_next; a = a_next;
        }
    }

    return den[n];
}
