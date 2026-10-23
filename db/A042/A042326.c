/*
 * A042326  Numerators of continued fraction convergents to sqrt(690).
 * https://oeis.org/A042326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 2942*a(n-6) - a(n-12), a(0)=26, a(1)=79, a(2)=105,
 *            a(3)=289, a(4)=394, a(5)=1471, a(6)=76886, a(7)=232129,
 *            a(8)=309015, a(9)=850159, a(10)=1159174, a(11)=4327681
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS A042326: numerators of convergents to sqrt(690). */
seq_t a(idx_t n)
{
    /* Static cache for memoisation of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid elements in the cache */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* --------- initialise cache with the first 12 terms --------- */
    if (!cache) {
        cache_cap = 12;
        cache_len = 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);   /* allocation failure */

        /* Compute the first 12 convergents of sqrt(690) via the
           standard continued–fraction recurrence.  */
        const int M = 690;                /* the irrational whose sqrt we use */
        const int a0_int = 26;            /* floor(sqrt(690)) */
        int a_int   = a0_int;             /* coefficient a_k */
        int m_int   = 0;                  /* m_k of the algorithm */
        int d_int   = 1;                  /* d_k of the algorithm */

        seq_t p_prevprev = 0;  /* p_{-2} */
        seq_t p_prev     = 1;  /* p_{-1} */

        for (idx_t k = 0; k < 12; ++k) {
            /* Compute the k‑th convergent numerator p_k = a_k * p_{k-1} + p_{k-2}. */
            seq_t p_k = a_int * p_prev + p_prevprev;
            cache[(size_t)k] = p_k;

            /* Prepare for the next coefficient a_{k+1} if needed. */
            int m_next = d_int * a_int - m_int;
            int d_next = (M - m_next * m_next) / d_int;

            if (k < 11) {                              /* need a_{k+1} for next step */
                a_int = (a0_int + m_next) / d_next;
                m_int = m_next;
                d_int = d_next;
            }

            /* Shift p_{k-1} and p_{k-2}. */
            p_prevprev = p_prev;
            p_prev = p_k;
        }
    }

    /* --------- extend cache if the requested n is beyond the current range --------- */
    if ((size_t)n >= cache_len) {
        size_t need = (size_t)n + 1;          /* number of elements needed (inclusive of n) */

        /* Resize cache if necessary. */
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < need)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms using the linear recurrence. */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[(size_t)i] = (seq_t)2942 * cache[(size_t)(i - 6)] - cache[(size_t)(i - 12)];
        }
        cache_len = need;
    }

    return cache[(size_t)n];
}
