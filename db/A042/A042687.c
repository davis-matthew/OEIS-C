/*
 * A042687  Denominators of continued fraction convergents to sqrt(873).
 * https://oeis.org/A042687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 125619266*a(n-18) - a(n-36) * Initial values a(0)..a(35)
 *            are the denominators of the convergents * to the continued
 *            fraction of sqrt(873), obtained by the standard * recurrence. *
 *            a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 9, a(4) = 11, a(5) = 75,
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;      /* array of computed values */
    static size_t cap   = 0;         /* capacity of `cache` */
    static size_t len   = 0;         /* number of values stored */
    static bool    ready = false;    /* have we initialized the first 36? */

    /* --------------------------------------------------------------------
       Initialise the first 36 terms (index 0..35) if not done yet
     -------------------------------------------------------------------- */
    if (!ready) {
        /* 873 = D, sqrt(873) ≈ 29.58  → a0 = floor = 29 */
        const int         D  = 873;
        const int         a0 = 29;
        const int         periods = 36;           /* need indices 0..35 */

        /* temporary storage for the first 36 terms */
        seq_t init[periods];

        /* next partial quotient variables */
        int m = 0, d = 1, a_prev = a0;

        /* k_{-2} = 1, k_{-1} = 0  (classical convergent recurrence) */
        seq_t k_nm2 = 1, k_nm1 = 0;
        seq_t k0   = (seq_t)a0 * k_nm1 + k_nm2;   /* a0*0 + 1 = 1 */
        init[0] = k0;
        k_nm2 = k_nm1;   /* 0   */
        k_nm1 = k0;      /* 1   */

        /* generate a1 .. a35 and corresponding denominators */
        for (int i = 1; i < periods; ++i) {
            /* next m and d, using integer arithmetic (exact division) */
            int m_next = d * a_prev - m;
            int d_next = (D - m_next * m_next) / d;
            int a_next = (a0 + m_next) / d_next;   /* partial quotient a_i */

            /* new denominator k_i = a_i * k_{i-1} + k_{i-2} */
            seq_t k_i = (seq_t)a_next * k_nm1 + k_nm2;

            init[i] = k_i;

            /* shift for next iteration */
            k_nm2 = k_nm1;
            k_nm1 = k_i;

            /* update m,d,a for next step */
            m      = m_next;
            d      = d_next;
            a_prev = a_next;
        }

        /* allocate cache with some slack */
        cap  = periods * 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < periods; ++i)
            cache[i] = init[i];
        len   = periods;
        ready = true;
    }

    /* --------------------------------------------------------------------
       Return value if already tabulated
     -------------------------------------------------------------------- */
    if (n < len) {
        return cache[(size_t)n];
    }

    /* --------------------------------------------------------------------
       Need to compute values up to index `n`.  Extend cache as required
     -------------------------------------------------------------------- */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* --------------------------------------------------------------------
       Compute missing terms using the recurrence
     -------------------------------------------------------------------- */
    for (size_t i = len; i <= (size_t)n; ++i) {
        /* a(i) = 125619266*a(i-18) - a(i-36)   for i >= 36 */
        seq_t val = (seq_t)125619266 * cache[i - 18] - cache[i - 36];
        cache[i] = val;
    }

    len = n + 1;      /* now we have cached up to `n` */
    return cache[(size_t)n];
}
