/*
 * A042646  Numerators of continued fraction convergents to sqrt(853).
 * https://oeis.org/A042646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 13, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = numerator of the n-th convergent to sqrt(853).
 * Initial value: a(0) = 29.
 * Recurrence for convergents: 
 *   p(-1) = 1, p(0) = 29,
 *   p(n) = a(n)*p(n-1) + p(n-2)   for n ≥ 1,
 * where a(n) are the continued–fraction terms of sqrt(853).
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the n-th term of OEIS sequence A042646 */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;

    /* continued‑fraction generation state (for sqrt(853)) */
    static uint64_t m_val = 0;          /* m_k value */
    static uint64_t d_val = 1;          /* d_k value */
    static uint64_t a_next_val = 29;    /* a_k value (a0 for k=0) */
    static const uint64_t a0 = 29;      /* floor(sqrt(853)) */

    /* recurrence state for convergents */
    static seq_t prev2 = 0; /* p(k-2) */
    static seq_t prev1 = 0; /* p(k-1) */

    /* initialise on first call */
    if (cache_size == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* allocation failed */

        cache[0] = (seq_t)29;               /* a(0) */
        cache_size = 1;

        prev2 = (seq_t)1;                   /* p(-1) = 1 */
        prev1 = cache[0];                   /* p(0) = 29 */
    }

    /* extend cache up to index n, if necessary */
    if ((size_t)n >= cache_size) {
        for (size_t idx = cache_size; idx <= (size_t)n; ++idx) {
            /* compute next continued‑fraction term */
            uint64_t m_next = d_val * a_next_val - m_val;
            uint64_t d_next = (853ULL - m_next * m_next) / d_val;
            uint64_t a_next = (a0 + m_next) / d_next;

            /* compute next convergent numerator */
            seq_t p_next = (seq_t)a_next * prev1 + prev2;

            /* ensure capacity */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                while (new_cap <= cache_size) new_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_size++] = p_next;

            /* update state for next iteration */
            m_val = m_next;
            d_val = d_next;
            a_next_val = a_next;
            prev2 = prev1;
            prev1 = p_next;
        }
    }

    return cache[(size_t)n];
}
