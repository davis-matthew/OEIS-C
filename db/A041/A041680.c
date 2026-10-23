/*
 * A041680  Numerators of continued fraction convergents to sqrt(359).
 * https://oeis.org/A041680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(359) *
 *            (continued fraction)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041680 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache for computed numerators */
static seq_t *p_cache   = NULL;   /* array of numerators p[n]   */
static size_t cache_cap = 0;      /* allocated capacity        */
static idx_t  max_n     = 0;      /* highest index already
                                              computed (0‑based)   */
static idx_t  cur_m     = 0;      /* current m in continued‑fraction
                                              recurrence            */
static idx_t  cur_d     = 1;      /* current d                     */
static idx_t  cur_a     = 18;     /* current a (a0 = floor(sqrt(359))) */
static const idx_t D  = 359;      /* D in sqrt(D) */
static const idx_t a0 = 18;       /* floor(sqrt(D)) */
static int    initialized = 0;    /* flag to perform one‑time init */

/* Ensure numerators up to index n are computed and cached */
static void
ensure_terms(idx_t n)
{
    if (!initialized) { /* first call – initialise cache */
        cache_cap = 8;               /* initial capacity */
        p_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!p_cache) exit(EXIT_FAILURE);
        p_cache[0] = (seq_t)a0;      /* p(0) = a0 */
        max_n = 0;
        initialized = 1;
    }

    while (max_n < n) {                 /* compute next few terms */
        /* next partial quotient a_{m+1} */
        idx_t m_next = cur_d * cur_a - cur_m;
        idx_t d_next = (D - (idx_t)((__uint128_t)m_next * m_next)) / cur_d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* recurrence for numerators: p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p_last = p_cache[max_n];
        seq_t p_prev2 = (max_n == 0 ? (seq_t)1 : p_cache[max_n-1]);

        seq_t p_next;
        if (max_n == 0) {
            p_next = (seq_t)a_next * p_last + (seq_t)1; /* p1 = a1*p0 + 1 */
        } else {
            p_next = (seq_t)a_next * p_last + p_cache[max_n-1];
        }

        /* expand cache if necessary */
        if (max_n + 1 >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            p_cache = tmp;
            cache_cap = new_cap;
        }

        p_cache[max_n + 1] = p_next;   /* store computed numerator */

        /* update continued‑fraction state for next step */
        cur_m = m_next;
        cur_d = d_next;
        cur_a = a_next;
        max_n++;                      /* now have terms up to max_n */
    }
}

/* Public function required by the assignment */
seq_t
a(idx_t n)
{
    ensure_terms(n);
    return p_cache[n];
}
