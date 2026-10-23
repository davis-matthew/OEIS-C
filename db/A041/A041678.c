/*
 * A041678  Numerators of continued fraction convergents to sqrt(358).
 * https://oeis.org/A041678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(358) */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static state to allow arbitrary order of calls and memoisation */
    static bool        initialized   = false;
    static seq_t      *cache         = NULL;
    static size_t      cache_len     = 0;   /* number of elements stored */
    static size_t      cache_cap     = 0;   /* allocated capacity             */
    static idx_t       m             = 0;   /* current m in continued fraction*/
    static idx_t       d             = 1;   /* current d                       */
    static idx_t       a_cur         = 18;  /* current a (a0)                 */
    static const idx_t a0            = 18;  /* floor(sqrt(358))                */

    /* Lazy initialisation: nothing to do until the first call to a(n)   */
    if (!initialized) {
        cache      = NULL;
        cache_len  = 0;
        cache_cap  = 0;
        m          = 0;
        d          = 1;
        a_cur      = a0;
        initialized = true;
    }

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms up to n */
    for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
        seq_t p_k;

        if (k == 0) {
            /* The first convergent is simply a0 / 1.  Numerator = a0. */
            p_k = (seq_t)a0;
            /* Continues fraction state remains the same for next step.  */
        } else {
            /* Next partial quotient a_k according to the continued fraction algorithm */
            idx_t m_next = d * a_cur - m;
            idx_t d_next = (358 - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            /* Update ongoing state for subsequent iterations */
            m       = m_next;
            d       = d_next;
            a_cur   = a_next;

            /* Recurrence for convergents: p_k = a_k * p_{k-1} + p_{k-2} */
            seq_t prev1 = cache[k - 1];
            seq_t prev2 = (k >= 2) ? cache[k - 2] : (seq_t)1;
            p_k = (seq_t)a_next * prev1 + prev2;
        }

        cache[k] = p_k;
    }

    cache_len = (size_t)(n + 1);
    return cache[(size_t)n];
}
