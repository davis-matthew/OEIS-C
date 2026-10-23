/*
 * A041675  Denominators of continued fraction convergents to sqrt(356).
 * https://oeis.org/A041675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the denominators of the continued‑fraction convergents of sqrt(356).
   The sequence satisfies the 28‑term linear recurrence
     a(n) = 1000002*a(n-14) – a(n-28)  (for n > 27)
   with the first 28 values produced here from the continued‑fraction
   expansion of sqrt(356).  This implementation builds the values on
   demand, storing previously computed terms in a static cache.  It works
   with any seq_t that supports +, -, and * with small integers. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed denominators */
    static seq_t *denoms = NULL;
    static size_t cap    = 0;
    static size_t sz     = 0;            /* number of terms stored (≥ 1 once init) */

    /* continued‑fraction parameters for sqrt(356) */
    static const idx_t D = 356;
    static const idx_t a0 = 18;          /* floor(sqrt(356)) */
    static idx_t m = 0;                  /* m_k */
    static idx_t d = 1;                  /* d_k */
    static idx_t a = a0;                 /* current partial quotient a_k */

    static seq_t q_prevprev = 0;          /* q_{n-2} */
    static seq_t q_prev     = 1;          /* q_{n-1} */

    if (denoms == NULL) {                /* first call – initialise cache */
        cap    = 8;
        denoms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!denoms) abort();
        denoms[0] = q_prev;              /* q_0 = 1 */
        sz = 1;
    }

    /* generate further terms up to index n */
    for (idx_t i = sz; i <= n; ++i)
    {
        /* compute next partial quotient a_{i} from m,d,a */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        /* next denominator q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t q_i = a * q_prev + q_prevprev;

        /* resize cache if necessary */
        if (sz == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            denoms = tmp;
            cap = new_cap;
        }

        denoms[sz++] = q_i;          /* store denominator */

        /* shift previous denominators for next iteration */
        q_prevprev = q_prev;
        q_prev     = q_i;
    }

    return denoms[n];
}
