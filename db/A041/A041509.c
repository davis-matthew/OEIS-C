/*
 * A041509  Denominators of continued fraction convergents to sqrt(271).
 * https://oeis.org/A041509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of sqrt(271)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the recurrence for the denominators q_k of the
 * convergents of the continued fraction expansion of sqrt(271).
 * It remembers the biggest index calculated so far and expands a
 * static array lazily.  The code works with both built‑in integral
 * types and a big integer class that supports the usual arithmetic
 * operators.  It is valid C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>   /* for uint64_t used in idx_t */

seq_t a(idx_t n)
{
    /* Constants for sqrt(271) */
    const int a0 = 16;          /* floor(sqrt(271)) */

    /* Static state to allow O(1) repeated calls. */
    static idx_t   last_computed = 0;   /* highest n computed so far */
    static seq_t * denoms        = NULL; /* array of q_k */
    static size_t denoms_cap     = 0;    /* allocated capacity */

    static idx_t   m_state = 0;   /* m_k in the CF algorithm */
    static idx_t   d_state = 1;   /* d_k in the CF algorithm */
    static int     a_prev  = a0;  /* a_{k-1} for next step (a0 for k=1) */

    /* ---------- initialisation ---------- */
    if (denoms == NULL) {               /* first call */
        /* allocate space for q_0 */
        denoms_cap = 1;
        denoms = (seq_t *)malloc(sizeof(seq_t) * denoms_cap);
        if (denoms == NULL) return 0;   /* allocation failed */
        denoms[0] = (seq_t)1;            /* q_0 = 1 */
        last_computed = 0;
    }

    /* ---------- guarantee capacity ---------- */
    if (denoms_cap <= (size_t)n + 1) {
        size_t new_cap = denoms_cap ? denoms_cap : 2;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;       /* allocation failed */
        denoms        = tmp;
        denoms_cap    = new_cap;
    }

    /* ---------- compute up to n (if necessary) ---------- */
    if (last_computed < n) {
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            /*----- continued‑fraction step -----*/
            idx_t m_next = d_state * a_prev - m_state;
            idx_t d_next = (271 - m_next * m_next) / d_state;
            int   a_next = (a0 + m_next) / d_next;

            /*----- denominator recursion q_k = a_k * q_{k-1} + q_{k-2} -----*/
            seq_t q_next;
            if (k == 1) {
                q_next = (seq_t)a_next * denoms[0];
            } else {
                q_next = (seq_t)a_next * denoms[k-1] + denoms[k-2];
            }

            /*----- store and advance state -----*/
            denoms[k]     = q_next;
            m_state       = m_next;
            d_state       = d_next;
            a_prev        = a_next;
            last_computed = k;
        }
    }

    return denoms[n];
}
