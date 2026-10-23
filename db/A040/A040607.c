/*
 * A040607  Continued fraction for sqrt(633).
 * https://oeis.org/A040607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A040607: Continued fraction for sqrt(633) – standard quadratic‑surd recurrence.
 * The sequence a(n) is the n‑th partial quotient of the simple continued fraction
 * expansion of sqrt(633).  It satisfies the recurrence:
 *
 *   a0 = 25, m0 = 0, d0 = 1
 *   mk   = dk‑1 * a(k‑1) - m(k‑1)
 *   dk   = (633 - mk²)/d(k‑1)
 *   a(k) = (25 + mk)/dk      for k ≥ 1
 *
 * The implementation memoises the sequence in a cache that grows as required.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache for computed terms and recurrence state */
    static seq_t *a_cache      = NULL;
    static seq_t *m_cache      = NULL;
    static seq_t *d_cache      = NULL;
    static size_t cap = 0;     /* allocated capacity */
    static size_t len = 0;     /* number of terms stored */

    /* initialise the cache on first call */
    if (a_cache == NULL) {
        cap = 16;                                   /* start with some space */
        a_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        m_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        d_cache = (seq_t *)malloc(cap * sizeof(seq_t));

        /* a0 = floor(sqrt(633)) = 25, m0 = 0, d0 = 1 */
        a_cache[0] = (seq_t)25;
        m_cache[0] = (seq_t)0;
        d_cache[0] = (seq_t)1;

        len = 1;                                   /* we have a(0) */
    }

    size_t N = (size_t)n;                            /* n fits in size_t for our limits */

    /* if already computed, return the cached value */
    if (N < len) {
        return a_cache[N];
    }

    /* grow the arrays to hold up to N */
    if (N + 1 > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= N) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        m_cache = (seq_t *)realloc(m_cache, new_cap * sizeof(seq_t));
        d_cache = (seq_t *)realloc(d_cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute new terms from len up to N */
    for (size_t i = len; i <= N; ++i) {
        /* previous values */
        idx_t m_prev = (idx_t)m_cache[i - 1];
        idx_t d_prev = (idx_t)d_cache[i - 1];
        idx_t a_prev = (idx_t)a_cache[i - 1];

        /* recurrence */
        idx_t m_new = d_prev * a_prev - m_prev;
        idx_t d_new = (633 - m_new * m_new) / d_prev;   /* 633 fits in idx_t */
        idx_t a_new = (25 + m_new) / d_new;              /* 25 is a0 */

        /* store the new values */
        m_cache[i] = (seq_t)m_new;
        d_cache[i] = (seq_t)d_new;
        a_cache[i] = (seq_t)a_new;
    }

    len = N + 1;                                      /* we now have a(0)…a(N) */
    return a_cache[N];
}
