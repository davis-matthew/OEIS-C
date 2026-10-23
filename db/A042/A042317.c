/*
 * A042317  Denominators of continued fraction convergents to sqrt(685).
 * https://oeis.org/A042317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042317:
   a(n) = denominator of the n‑th convergent of the continued fraction 
   of sqrt(685).  Recurrence: q(-1)=0, q(0)=1,
   q(k)=a_k*q(k-1)+q(k-2), where 
      a_0 = 26 and for k≥0 
          m_{k+1}=d_k*a_k−m_k, 
          d_{k+1}=(685−m_{k+1}^2)/d_k, 
          a_{k+1}=⌊(26+m_{k+1})/d_{k+1}⌋. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of denominators */
    static seq_t *cache  = NULL;
    static size_t cap    = 0;
    static size_t sz     = 0;  /* next free index */

    /* continued fraction state for sqrt(685) */
    static uint64_t m     = 0;  /* m_k          (k current index) */
    static uint64_t d     = 1;  /* d_k          */
    static uint64_t a_val = 26; /* a_k          */

    /* recurrence for denominators */
    static seq_t q_prev2 = 0;  /* q(k-2) */
    static seq_t q_prev1 = 1;  /* q(k-1) */

    /* one‑time initialization */
    if (!cache) {
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;               /* out of memory */
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        sz       = 1;                       /* next index to compute */
    }

    /* extend cache until index n is available */
    while (sz <= n) {
        /* compute next partial quotient a_{k+1} */
        uint64_t m_next = d * a_val - m;
        uint64_t d_next = (685ULL - m_next * m_next) / d;
        uint64_t a_next = (26ULL + m_next) / d_next;

        /* compute next denominator q_k+1 */
        seq_t a_next_seq = (seq_t)a_next;
        seq_t q_next     = a_next_seq * q_prev1 + q_prev2;

        /* grow cache if necessary */
        if (sz >= cap) {
            cap = cap ? cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) return 0;                  /* out of memory */
            cache = tmp;
        }
        cache[sz++] = q_next;

        /* shift recurrence variables */
        q_prev2 = q_prev1;
        q_prev1 = q_next;

        /* advance continued‑fraction state */
        m     = m_next;
        d     = d_next;
        a_val = a_next;
    }

    return cache[n];
}
