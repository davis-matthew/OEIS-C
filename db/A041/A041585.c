/*
 * A041585  Denominators of continued fraction convergents to sqrt(310).
 * https://oeis.org/A041585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 26, u64 n <= 51, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041585: Denominators of continued fraction convergents to sqrt(310).
   The denominators q_n of the convergents satisfy
   q_n = a_n * q_{n-1} + q_{n-2},  with q_0 = 1, q_{-1} = 0.
   The partial quotients a_n are obtained from the periodic continued fraction
   expansion of sqrt(310).  A static cache is used so that any order of calls
   is supported. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;              /* number of terms currently stored (len >= 1) */

    /* state for generating continued-fraction partial quotients for sqrt(310) */
    static uint64_t m_cur = 0;          /* m_k          */
    static uint64_t d_cur = 1;          /* d_k          */
    static idx_t    a_cur = 17;         /* a_k (a0)     */
    static seq_t   denom_prevprev = 0;  /* q_{-1}       */
    static seq_t   denom_prev     = 1;  /* q_0          */

    const uint64_t D  = 310;            /* discriminant */
    const idx_t    a0 = 17;             /* floor(sqrt(310)) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;           /* q_0 = 1 */
        len = 1;
    }

    /* return cached value if already computed */
    if (n < len) {
        return cache[n];
    }

    /* extend the cache up to the requested index */
    while (len <= n) {
        /* next partial quotient a_{k+1} */
        uint64_t m_next = d_cur * a_cur - m_cur;
        uint64_t d_next = (D - m_next * m_next) / d_cur;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* next denominator q_{k+1} = a_next * q_k + q_{k-1} */
        seq_t q_new = (seq_t)a_next * denom_prev + denom_prevprev;

        /* reallocate cache if needed */
        if (len >= cap) {
            cap <<= 1;                    /* double capacity */
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        }
        cache[len++] = q_new;

        /* shift denominator history for next iteration */
        denom_prevprev = denom_prev;
        denom_prev     = q_new;

        /* update continued‑fraction state for next iteration */
        m_cur = m_next;
        d_cur = d_next;
        a_cur = (idx_t)a_next;
    }

    return cache[n];
}
