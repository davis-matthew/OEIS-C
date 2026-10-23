/*
 * A042465  Denominators of continued fraction convergents to sqrt(760).
 * https://oeis.org/A042465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(n) = 104042*a(n-10) - a(n-20)  (for n ≥ 20)
 * Initial values are the first 20 denominators of the continued‑fraction
 * convergents to √760, computed via the standard recurrence.
 */

#include <stdlib.h>

/* Computes a(n) for the OEIS sequence A042465. */
seq_t a(idx_t n)
{
    /* Cache of already computed values.  `computed` is the first
       index that has not yet been assigned. */
    static seq_t *cache      = NULL;  /* cache[0..computed-1] are valid   */
    static idx_t cache_len   = 0;     /* allocated size in elements        */
    static idx_t computed    = 0;     /* next index to compute             */

    /* ---------- initialise the first 20 terms via continued
       fraction expansion for √760 ---------- */
    if (!cache) {
        cache_len = 20;
        cache     = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure */

        /* Continued‑fraction data for √760. */
        const idx_t D    = 760;
        const idx_t a0   = 27;          /* floor(√760) */
        idx_t        m    = 0;
        idx_t        d    = 1;
        idx_t        a    = a0;

        /* Previous two convergent numerators/denominators. */
        seq_t p_prev2 = 0;   /* p_{-2} */
        seq_t p_prev1 = 1;   /* p_{-1} */
        seq_t q_prev2 = 1;   /* q_{-2} */
        seq_t q_prev1 = 0;   /* q_{-1} */

        /* Generate the first 20 denominators q_k. */
        for (idx_t k = 0; k < 20; ++k) {
            idx_t ak = a;  /* coefficient a_k */

            /* p_k = a_k*p_{k-1} + p_{k-2}   (not needed) */
            seq_t p = ((seq_t)ak) * p_prev1 + p_prev2;
            /* q_k = a_k*q_{k-1} + q_{k-2} */
            seq_t q = ((seq_t)ak) * q_prev1 + q_prev2;

            cache[k] = q;

            /* shift for next iteration */
            p_prev2 = p_prev1;  p_prev1 = p;
            q_prev2 = q_prev1;  q_prev1 = q;

            /* prepare next coefficient, if any */
            if (k + 1 < 20) {
                idx_t m_next = d * ak - m;
                idx_t d_next = (D - m_next * m_next) / d;
                idx_t a_next = (a0 + m_next) / d_next;
                m = m_next;  d = d_next;  a = a_next;
            }
        }

        computed = 20;   /* indices 0..19 are now ready */
    }

    /* ---------- enlarge the cache if the caller wants a larger index ----------
       (realloc never shrinks) */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* ---------- compute any missing terms up to n using the linear recurrence ----------
       (for indices < 20 the values are already correct) */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i < 20) {
                /* should never happen: the first 20 are already set */
                continue;
            }
            cache[i] = ((seq_t)104042) * cache[i - 10] - cache[i - 20];
        }
        computed = n + 1;
    }

    return cache[n];
}
