/*
 * A041458  Numerators of continued fraction convergents to sqrt(245).
 * https://oeis.org/A041458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A041458 – Numerators of continued‑fraction convergents to sqrt(245)
 *
 *  Formula (for n ≥ 20):
 *      a(n) = 103682 * a(n‑10) – a(n‑20)
 *
 *  The first 20 terms are obtained by computing the continued fraction
 *  expansion of √245.  After those are known the recurrence is used for
 *  all larger indices.  Results are memoised so that arbitrary orders
 *  of calls are supported efficiently.
 */

#include <stdlib.h>
#include <stdint.h>

static void compute_base_terms(seq_t *term)
{
    /* Continued‑fraction data for sqrt(245) */
    const int D = 245;
    const int a0 = 15;          /* floor(sqrt(245)) */

    int64_t m = 0, d = 1, a = a0;
    seq_t p_prev2 = 0, p_prev = 1;   /* p_{-2}, p_{-1} */
    seq_t q_prev2 = 1, q_prev = 0;   /* q_{-2}, q_{-1} */

    for (idx_t k = 0; k < 20; ++k) {
        /* p_k = a_k * p_{k‑1} + p_{k‑2} */
        seq_t p = (seq_t)a * p_prev + p_prev2;
        /* q_k = a_k * q_{k‑1} + q_{k‑2}  (not used after this loop) */
        (void)q_prev;                /* silence unused warning */
        (void)q_prev2;
        /* store numerator */
        term[k] = p;

        /* Prepare for next partial quotient */
        int64_t m_next = d * a - m;
        int64_t d_next = (D - m_next * m_next) / d;
        int64_t a_next = (a0 + m_next) / d_next;

        /* shift recurrence variables */
        p_prev2 = p_prev;
        p_prev  = p;
        q_prev2 = q_prev;
        q_prev  = (seq_t)a * q_prev + q_prev2;

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

seq_t a(idx_t n)
{
    /* memoised table of already computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;   /* number of initialized entries */
    static size_t cache_cap = 0;   /* allocated capacity (in terms) */

    /* first call – initialise with the first 20 terms */
    if (cache_len == 0) {
        cache_cap = 20;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory – return 0 */
        compute_base_terms(cache);
        cache_len = 20;
    }

    /* ensure the table is large enough for the requested index */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out of memory – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms using the recurrence */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 20) continue;    /* should not happen – 20 terms are pre‑computed */
            cache[i] = (seq_t)103682 * cache[i - 10] - cache[i - 20];
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
