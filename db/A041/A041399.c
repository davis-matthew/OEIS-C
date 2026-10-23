/*
 * A041399  Denominators of continued fraction convergents to sqrt(214).
 * https://oeis.org/A041399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): Denominators of continued‑fraction convergents
   to sqrt(214).  The numerator and denominator of the k‑th
   convergent are produced by the recurrence:
     p_(-1)=1, p_0=a0
     q_(-1)=0, q_0=1
     p_k = a_k * p_(k-1) + p_(k-2)
     q_k = a_k * q_(k-1) + q_(k-2)
   with a0=floor(sqrt(214))=14 and a_k obtained from the
   standard Pell‑equation recurrence.  This algorithm
   maintains the necessary state and memoises the sequence
   for repeated calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *den = NULL;      /* array of computed denominators */
    static size_t cache_size = 0;  /* number of elements stored */
    static size_t cache_cap   = 0;  /* current capacity of den */

    /* continued fraction state (only small integers) */
    static uint64_t m = 0;          /* m_k in recurrence */
    static uint64_t d = 1;          /* d_k in recurrence */
    static uint64_t a_prev = 14;    /* a_k, starting with a0 */

    /* previous two denominator terms */
    static seq_t q_minus2 = 0;      /* q_(k-2) */
    static seq_t q_minus1 = 1;      /* q_(k-1) */

    /* initialise first term if this is the first call */
    if (cache_size == 0) {
        cache_cap = 1;
        den = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        den[0] = (seq_t)1;          /* a(0) = 1 */
        cache_size = 1;
    }

    /* if the requested index is already computed, return it */
    if (n < cache_size)
        return den[n];

    /* extend the cache up to index n */
    for (size_t k = cache_size; k <= n; ++k) {
        /* compute next coefficient a_k using the Pell recurrence */
        uint64_t m_next = d * a_prev - m;
        uint64_t d_next = (214ULL - m_next * m_next) / d;
        uint64_t a_next = (14ULL + m_next) / d_next;

        /* update state for next iteration */
        m      = m_next;
        d      = d_next;
        a_prev = a_next;

        /* compute the new denominator q_k */
        seq_t q_new = (seq_t)a_prev * q_minus1 + q_minus2;

        /* grow the storage if necessary */
        if (cache_size >= cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            den = (seq_t *)realloc(den, cache_cap * sizeof(seq_t));
        }

        /* store and advance the history of denominators */
        den[cache_size++] = q_new;
        q_minus2 = q_minus1;
        q_minus1 = q_new;
    }

    return den[n];
}
