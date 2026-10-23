/*
 * A042795  Denominators of continued fraction convergents to sqrt(928).
 * https://oeis.org/A042795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued fraction convergents to sqrt(928) /*
 *            a(0) = 1, a(1) = 2; for n >= 2: a(n) = p_n*a(n-1) + a(n-2) *
 *            where p_n is the n-th partial quotient of √928
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache and state */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static seq_t prev1 = 0, prev2 = 1; /* prev1 = q_{k-1}, prev2 = q_{k-2} */
    static idx_t  m      = 0;
    static idx_t  d      = 1;
    static idx_t  a_cur  = 30;            /* a_k, initial a_0 = floor(sqrt(928)) = 30 */

    static const idx_t S   = 928;
    static const idx_t a0  = 30;          /* floor(sqrt(928)) */

    /* initialisation on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;            /* out of memory – return 0 */
        cache[0] = (seq_t)1;             /* a(0) */
        cache_len = 1;
        /* prev1/q_{k-1} = q_0 = 1,  prev2/q_{k-2} = q_{-1} = 0 */
        prev1 = (seq_t)1;
        prev2 = (seq_t)0;
        m     = 0;
        d     = 1;
        a_cur = a0;                      /* a_0 */
    }

    /* return cached value if already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* compute next partial quotient a_{i} (for index i) */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (S - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;   /* floor division */

        /* compute denominator of convergent q_i */
        seq_t q = a_next * prev1 + prev2;

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return 0;                    /* out of memory */
            cache    = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = q;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = q;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    return cache[n];
}
