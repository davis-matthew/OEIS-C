/*
 * A041808  Numerators of continued fraction convergents to sqrt(425).
 * https://oeis.org/A041808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041808: numerators of continued fraction convergents to sqrt(425). */
/* Recurrence: p_{-2}=0, p_{-1}=1.
   Partial quotients a_n of sqrt(425) are produced by the classic algorithm:
   m_{k+1}=d_k*a_k-m_k,
   d_{k+1}=(425-m_{k+1}^2)/d_k,
   a_{k+1}=⌊(⌊√425⌋+m_{k+1})/d_{k+1}⌋, with a_0=20.
   Then p_n = a_n * p_{n-1} + p_{n-2}. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of computed numerators */
    static seq_t *num_cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* continued‑fraction state */
    static uint64_t m = 0;          /* m_k */
    static uint64_t d = 1;          /* d_k */
    static const uint64_t a0 = 20;  /* floor(sqrt(425)) */
    static uint64_t a_prev = 20;    /* a_k used in the recurrence */

    /* recurrence for numerators */
    static seq_t p_prev2 = 1;   /* p_{-1} */
    static seq_t p_prev1 = 20;  /* p_0 */

    /* initialise on first call */
    if (cache_len == 0)
    {
        cache_cap = 16;
        num_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        num_cache[0] = (seq_t)a0;   /* p_0 = a_0 */
        cache_len = 1;
    }

    /* generate values up to required n */
    while (n >= cache_len)
    {
        /* compute next partial quotient a_{k} */
        uint64_t m_next = d * a_prev - m;
        uint64_t d_next = (425 - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* compute next numerator p_k = a_next * p_prev1 + p_prev2 */
        seq_t term = p_prev1 * (seq_t)a_next;
        term = term + p_prev2;

        /* ensure capacity */
        if (cache_len >= cache_cap)
        {
            cache_cap = cache_cap * 2;
            num_cache = (seq_t*)realloc(num_cache, cache_cap * sizeof(seq_t));
        }
        num_cache[cache_len] = term;
        cache_len++;

        /* update state for next iteration */
        m = m_next;
        d = d_next;
        a_prev = a_next;
        p_prev2 = p_prev1;
        p_prev1 = term;
    }

    return num_cache[n];
}
