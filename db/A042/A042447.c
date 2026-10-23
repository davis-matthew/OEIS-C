/*
 * A042447  Denominators of continued fraction convergents to sqrt(751).
 * https://oeis.org/A042447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Denominators of continued‑fraction convergents to sqrt(751).
 * Recurrence:
 *     q_{-1} = 0,  q_0 = 1
 *     q_n   = a_n * q_{n-1} + q_{n-2}   (n ≥ 1)
 * where the partial quotients a_n are generated from the continued
 * fraction of sqrt(751) by the Pell‑equation recurrence.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache for previously computed terms */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  sz     = 0;

    /* continued‑fraction parameters (all integer types) */
    static idx_t  m     = 0;   /* current m in the CF recursion   */
    static idx_t  d     = 1;   /* current d in the CF recursion   */
    static idx_t  a_cur = 27;  /* a_k at the current step (a_0)  */

    /* recurrence state for the denominators */
    static seq_t q_prevprev = 0; /* q_{k-2}   */
    static seq_t q_prev     = 1; /* q_{k-1}   */

    const idx_t S  = 751;   /* the number under the square root */
    const idx_t a0 = 27;    /* floor(sqrt(751))                  */

    /* initialise cache on first call */
    if (sz == 0) {
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();
        cap  = init_cap;
        sz   = 1;
        cache[0] = (seq_t)1;   /* a(0) = 1 */
    }

    /* grow cache if needed */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute terms until we reach the requested index n */
    for (size_t idx = sz; idx <= (size_t)n; ++idx) {
        /* generate next partial quotient a_next (starting from a_1) */
        idx_t m_next  = d * a_cur - m;
        idx_t d_next  = (S - m_next * m_next) / d;
        idx_t a_next  = (a0 + m_next) / d_next;

        /* compute the next denominator q_k */
        seq_t q_next = (seq_t)a_next * q_prev + q_prevprev;
        cache[sz++] = q_next;

        /* advance CF state for the next step */
        m      = m_next;
        d      = d_next;
        a_cur  = a_next;

        /* shift the q recurrence buffers */
        q_prevprev = q_prev;
        q_prev     = q_next;
    }

    return cache[(size_t)n];
}
