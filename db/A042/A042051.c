/*
 * A042051  Denominators of continued fraction convergents to sqrt(549).
 * https://oeis.org/A042051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the nth convergent to sqrt(549)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iterating the continued‑fraction expansion of sqrt(549). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Continued‑fraction state variables */
    static idx_t m = 0;             /* current m */
    static idx_t d = 1;             /* current d */
    static idx_t a_coeff = 23;       /* current partial quotient a_k (k≥1) */

    /* Denominator history for the recurrence q_k = a_k*q_{k-1} + q_{k-2} */
    static seq_t q_prev2 = 0;        /* q_{k-2} */
    static seq_t q_prev  = 1;        /* q_{k-1} */

    const idx_t S   = 549;           /* integer whose square root is expanded */
    const idx_t A0  = 23;            /* floor(sqrt(S)) */

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Allocate cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0; /* allocation failure – unlikely in tests */
    }

    /* Generate terms up to index n */
    while (cache_len <= n) {
        if (cache_len == 0) {
            /* First term: q_0 = 1 */
            cache[0] = q_prev;          /* q_prev holds 1 */
            cache_len = 1;

            /* Advance to obtain a_1 for the next step */
            idx_t m_next = d * a_coeff - m;
            idx_t d_next = (S - m_next * m_next) / d;
            idx_t a_next = (A0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a_coeff = a_next;
        } else {
            /* Compute q_k for current k = cache_len */
            seq_t q = (seq_t)a_coeff * q_prev + q_prev2;

            /* Shift denominator history */
            q_prev2 = q_prev;
            q_prev  = q;

            /* Resize cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len] = q;
            cache_len++;

            /* Compute next partial quotient a_{k+1} */
            idx_t m_next = d * a_coeff - m;
            idx_t d_next = (S - m_next * m_next) / d;
            idx_t a_next = (A0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a_coeff = a_next;
        }
    }

    return cache[n];
}
