/*
 * A041982  Numerators of continued fraction convergents to sqrt(514).
 * https://oeis.org/A041982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9250*a(n-6) - a(n-12) for n >= 12.  
   The initial values a(0)…a(11) are the numerators of the convergents
   of the continued fraction of sqrt(514).  They are generated at first
   call by the standard continued‑fraction algorithm. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* next unused index */
    static size_t cache_cap  = 0; /* allocated capacity */

    /* helper to grow the cache */
    if (cache == NULL) {
        /* start with a small capacity */
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute the first 12 terms by continued fraction of sqrt(514) */
    if (cache_size < 12) {
        const int period = 12;
        const unsigned int D = 514;
        const unsigned int a0 = 22;               /* floor(sqrt(514)) */
        unsigned int m = 0;
        unsigned int d = 1;
        unsigned int a = a0;

        seq_t p_minus1 = 1;               /* p_{-1} */
        seq_t p_curr   = a0;              /* p_0 */
        cache[0] = p_curr;

        for (int i = 1; i < period; ++i) {
            unsigned int m_next = d * a - m;
            unsigned int d_next = (D - m_next * m_next) / d;
            unsigned int a_next = (a0 + m_next) / d_next;

            seq_t p_next = (seq_t)a_next * p_curr + p_minus1;
            cache[i] = p_next;

            /* shift for next step */
            p_minus1 = p_curr;
            p_curr   = p_next;
            m = m_next;
            d = d_next;
            a = a_next;
        }
        cache_size = period;
    }

    /* extend the cache up to index n using the recurrence */
    if ((size_t)n + 1 > cache_size) {
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            /* recurrence only applies for i >= 12, but i never < 12 here */
            cache[i] = (seq_t)9250 * cache[i - 6] - cache[i - 12];
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
