/*
 * A041255  Denominators of continued fraction convergents to sqrt(139).
 * https://oeis.org/A041255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 155126500*a(n-18) - a(n-36) for n > 35
   Initial values a(0)…a(35) are the denominators of the convergents
   of the continued fraction for sqrt(139). These are obtained by
   generating the continued‑fraction expansion and computing the
   standard convergents.  This implementation memoises the values
   so that calls can arrive in arbitrary order. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of values currently stored */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* helper to grow cache */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise first 36 terms (indices 0…35) once */
    if (cache_size == 0) {
        /* continued–fraction parameters for sqrt(139) */
        const uint64_t D = 139;
        const uint64_t a0 = 11;                 /* floor(sqrt(139)) */
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a = a0;

        /* convergent recurrence: q_-2 = 1, q_-1 = 0 */
        seq_t q_prev2 = 1;   /* q[-2] */
        seq_t q_prev1 = 0;   /* q[-1] */

        for (size_t k = 0; k < 36; ++k) {
            /* current denominator */
            seq_t q = q_prev1 * (seq_t)a + q_prev2;
            cache[k] = q;

            /* advance for next term, except after the last one */
            if (k == 35)
                break;

            uint64_t m_next = d * a - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            uint64_t a_next = (a0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a = a_next;

            /* shift for next iteration */
            q_prev2 = q_prev1;
            q_prev1 = q;
        }
        cache_size = 36;
    }

    /* If requested index is already cached, return */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Compute remaining values using the recurrence
       a(n) = 155126500*a(n-18) – a(n-36)  for n > 35 */
    for (size_t k = cache_size; k <= (size_t)n; ++k) {
        seq_t prod = cache[k - 18];
        prod *= (seq_t)155126500;
        prod -= cache[k - 36];
        cache[k] = prod;
    }
    cache_size = (size_t)n + 1;

    return cache[n];
}
