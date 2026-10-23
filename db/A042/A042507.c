/*
 * A042507  Denominators of continued fraction convergents to sqrt(781).
 * https://oeis.org/A042507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 135212398*a(n-18) - a(n-36) for n>35 Initial values a(0)
 *            .. a(35) are the first 36 denominators of convergents of
 *            sqrt(781)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 21, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build */
seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache      = NULL;
    static size_t    cache_cap   = 0;
    static idx_t     cache_last  = 0;   /* largest index stored (>=0) */

    /* --- initialization, compute first 36 terms --- */
    if (cache_cap == 0) {
        /* allocate some space, will grow if needed */
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);

        /* compute first 36 convergents of sqrt(781) */
        const long long D = 781;
        /* integer sqrt of D */
        long long a0 = 0;
        while ((a0 + 1) * (a0 + 1) <= D) ++a0;

        long long m = 0, d = 1, a_coef = a0;

        seq_t q_prev2 = 1; /* q_{-2} */
        seq_t q_prev1 = 0; /* q_{-1} */
        seq_t q_curr;

        for (idx_t i = 0; i < 36; ++i) {
            /* q_i = a_i * q_{i-1} + q_{i-2} */
            q_curr = (seq_t)a_coef * q_prev1 + q_prev2;
            cache[i] = q_curr;

            /* prepare for next coefficient */
            q_prev2 = q_prev1;
            q_prev1 = q_curr;

            long long m_next  = d * a_coef - m;
            long long d_next  = (D - m_next * m_next) / d;
            long long a_next  = (a0 + m_next) / d_next;  /* floor division */

            m = m_next;
            d = d_next;
            a_coef = a_next;
        }
        cache_last = 35;
    }

    /* --- ensure cache large enough --- */
    if (n > cache_last) {
        /* grow cache to at least n+1 elements */
        size_t needed = n + 1;
        if (needed > cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 64;
            if (newcap < needed) newcap = needed;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = newcap;
        }

        /* compute subsequent terms using the 18‑term recurrence */
        const seq_t coef = (seq_t)135212398;
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            if (i <= 35) {
                /* already computed during initialization */
                continue;
            } else {
                cache[i] = coef * cache[i - 18] - cache[i - 36];
            }
        }
        cache_last = n;
    }

    return cache[n];
}
