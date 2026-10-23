/*
 * A042425  Denominators of continued fraction convergents to sqrt(740).
 * https://oeis.org/A042425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042425: denominators of convergents to sqrt(740).
   Recurrence (continued‑fraction based): 
   q_{-1}=0, q_{0}=1;
   for k≥0:
       m_{k+1}=d_k·a_k – m_k,
       d_{k+1}=(D – m_{k+1}²)/d_k,
       a_{k+1}=⌊(A0 + m_{k+1})/d_{k+1}⌋,
       q_{k+1}=a_{k+1}·q_k + q_{k-1};
   The function returns q_n  (with n=0 giving q_0=1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(740) */
    static const seq_t D     = 740;      /* the integer under the sqrt   */
    static const seq_t A0    = 27;       /* floor(sqrt(740))             */

    /* static cache of computed denominators */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* continued‑fraction state variables (kept across calls) */
    static seq_t m      = 0;   /* m_k   of algorithm          */
    static seq_t d      = 1;   /* d_k   of algorithm          */
    static seq_t a_partial = A0; /* a_k   of algorithm         */

    /* denominator recurrence state (kept across calls) */
    static seq_t q_prev_prev = 1; /* q_{k-2}                     */
    static seq_t q_prev     = 0; /* q_{k-1}                     */

    /* initialise cache on first use */
    if (cache_len == 0 && cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
    }

    /* grow cache to hold up to index n */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* generate missing terms up to index n */
    for (size_t i = cache_len; i < needed; ++i) {
        /* current denominator q_i */
        seq_t tmp = a_partial * q_prev + q_prev_prev;
        cache[i] = tmp;

        /* prepare for next partial quotient */
        seq_t m_next = d * a_partial - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        /* shift recurrence state */
        q_prev_prev = q_prev;
        q_prev     = cache[i];

        /* update continued‑fraction state */
        m          = m_next;
        d          = d_next;
        a_partial  = a_next;
    }

    cache_len = needed;
    return cache[(size_t)n];
}
