/*
 * A042581  Denominators of continued fraction convergents to sqrt(819).
 * https://oeis.org/A042581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 59, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator of the nth convergent of the continued‑fraction expansion of sqrt(819) */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Constants for sqrt(819) */
    static const idx_t D   = 819;   /* D = 819          */
    static const idx_t a0  = 28;    /* a0 = floor(sqrt(D)) */

    /* Static cache for denominators (q_k) */
    static bool   initialized = false;
    static seq_t *cache       = NULL;
    static idx_t  cache_len   = 0;
    static idx_t  cur_n       = 0;   /* highest index computed so far */

    /* Static state for the continued‑fraction recurrence */
    static seq_t p_prev, p_prev2;    /* p_{k-1}, p_{k-2}   */
    static seq_t q_prev, q_prev2;    /* q_{k-1}, q_{k-2}   */
    static idx_t  m_next, d_next;    /* parameters for next step   */
    static idx_t  a_next;            /* a_{k} for next step       */

    /* First‑time initialisation (k = 0) */
    if (!initialized)
    {
        /* cache for q0 */
        cache_len = 1;
        cache     = (seq_t*)malloc(sizeof(seq_t));
        cache[0]  = (seq_t)1;          /* q0 = 1 */

        /* continued‑fraction data for k = 0 */
        cur_n    = 0;
        p_prev   = (seq_t)a0;          /* p0   = a0   */
        p_prev2  = (seq_t)1;           /* p-1  = 1    */
        q_prev   = (seq_t)1;           /* q0   = 1    */
        q_prev2  = (seq_t)0;           /* q-1  = 0    */

        /* compute a1 (coefficient for k = 1) */
        idx_t m1 = a0;                   /* m1 = d0*a0 - m0   (m0=0,d0=1) */
        idx_t d1 = (D - m1*m1);          /* d1 = (D - m1^2)/d0 (since d0=1) */
        idx_t a1 = (a0 + m1) / d1;       /* a1 = (a0 + m1)/d1 */

        m_next = m1;
        d_next = d1;
        a_next = a1;

        initialized = true;
    }

    /* Return cached value if already computed */
    if (n <= cur_n)
        return cache[n];

    /* Compute further terms up to n */
    while (cur_n < n)
    {
        /* next numerator and denominator */
        seq_t p_next = (seq_t)a_next * p_prev + p_prev2;
        seq_t q_next = (seq_t)a_next * q_prev + q_prev2;

        /* resize cache if necessary */
        if (cur_n + 1 >= cache_len)
        {
            size_t new_len = cache_len ? cache_len : 1;
            while (new_len <= cur_n + 1)
                new_len <<= 1;
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
            cache_len = new_len;
        }
        cache[cur_n + 1] = q_next;

        /* shift recurrence state */
        p_prev2 = p_prev;
        p_prev  = p_next;
        q_prev2 = q_prev;
        q_prev  = q_next;

        /* compute next coefficient a_{k+1} */
        idx_t m_next_next = d_next * a_next - m_next;
        idx_t d_next_next = (D - m_next_next * m_next_next) / d_next;
        idx_t a_next_next = (a0 + m_next_next) / d_next_next;

        m_next = m_next_next;
        d_next = d_next_next;
        a_next = a_next_next;

        cur_n++;
    }

    return cache[n];
}
