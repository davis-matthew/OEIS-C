/*
 * A040442  Continued fraction for sqrt(464).
 * https://oeis.org/A040442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Continued fraction coefficients for √464.
 * a(0) = floor(sqrt(464)) = 21.
 * For n > 0 the coefficients are produced by the standard recurrence
 * for the continued fraction of a quadratic irrational:
 *
 *   m_0 = 0,   d_0 = 1,   a_0 = 21
 *   m_{k+1} = d_k * a_k - m_k
 *   d_{k+1} = (464 - m_{k+1}^2) / d_k          (always integer)
 *   a_{k+1} = floor((21 + m_{k+1}) / d_{k+1})
 *
 * The terms are stored in a cache so that repeated calls are O(1)
 * except for the first time each new term is computed.
 */

#include <stdlib.h>
/* seq_t and idx_t are provided by the build. */

seq_t a(idx_t n)
{
    /* Cached terms of the continued fraction of √464 */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* State variables of the recurrence – remain across calls */
    static seq_t m      = 0;                     /* m_k */
    static seq_t d      = 1;                     /* d_k */
    static seq_t a_prev = 21;                    /* a_k */

    /* Constants as seq_t so that the code works for big integers */
    static const seq_t D  = 464;                 /* radicand */
    static const seq_t a0 = 21;                  /* floor(sqrt(D)) */

    /* Initialise the cache on first call */
    if (!cache) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = a0;
        len     = 1;
    }

    /* Ensure the cache can hold up to index n */
    size_t idx = (size_t)n;
    if (idx >= len) {
        size_t needed = idx + 1;
        if (cap < needed) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute missing terms up to n */
        for (size_t i = len; i <= idx; ++i) {
            seq_t m_next = d * a_prev - m;
            seq_t d_next = (D - m_next * m_next) / d;   /* integer division */
            seq_t a_new  = (a0 + m_next) / d_next;      /* floor division */

            cache[i] = a_new;

            /* Advance the recurrence state */
            m      = m_next;
            d      = d_next;
            a_prev = a_new;
        }
        len = needed;
    }

    return cache[idx];
}
