/*
 * A041708  Numerators of continued fraction convergents to sqrt(374).
 * https://oeis.org/A041708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = Numerator of the nth convergent in the continued‑fraction
 *            expansion of sqrt(374). The terms are obtained by the standard
 *            recurrence for continued fractions with initial term
 *            floor(sqrt(374))
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence.  cache[0] = a(0).  */
    static seq_t *cache      = NULL;
    static size_t  cache_len = 0;   /* number of elements stored   */
    static size_t  cache_cap = 0;   /* allocated capacity in elems   */

    /* Continued‑fraction state variables that are kept between calls. */
    static seq_t m   = 0;          /* current m_k   */
    static seq_t d   = 1;          /* current d_k   */
    static seq_t cf_a = 0;         /* current a_k   */
    static seq_t p_prev2 = 1;      /* p_{k-2}       */
    static seq_t p_prev1 = 0;      /* p_{k-1}       */
    static seq_t a0 = 0;           /* floor(sqrt(374)) */

    /* Constant D = 374 as a seq_t. */
    static const seq_t D = (seq_t)374ULL;

    /* Initialise everything on the first call. */
    if (cache == NULL) {
        /* Allocate a small initial cache. */
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */

        /* Compute a0 = floor(sqrt(374)) using integer arithmetic. */
        /* We use 64‑bit arithmetic for the small constant 374. */
        uint64_t root = 0;
        while ((root + 1) * (root + 1) <= 374ULL) ++root;
        a0 = (seq_t)root;

        /* First element of the sequence is a0. */
        cache[0] = a0;
        cache_len = 1;

        /* State for the next term (k = 1). */
        m      = 0;
        d      = 1;
        cf_a   = a0;      /* a_0          */
        p_prev1 = a0;     /* p_0          */
        p_prev2 = (seq_t)1; /* p_{-1}      */
    }

    /* If the requested index is already in the cache, return it directly. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Ensure that the cache is large enough to hold up to n inclusive. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the continued‑fraction terms up to index n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Generate the next partial quotient a_{i}. */
        seq_t m_next = d * cf_a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Update state for the next iteration. */
        m = m_next;
        d = d_next;
        cf_a = a_next;

        /* Compute the numerator of the convergent using
           p_i = a_i * p_{i-1} + p_{i-2}. */
        seq_t p = cf_a * p_prev1 + p_prev2;
        cache[i] = p;

        /* Shift the previous two numerators for the next step. */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
