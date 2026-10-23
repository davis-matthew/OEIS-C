/*
 * A040856  Continued fraction for sqrt(886).
 * https://oeis.org/A040856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(886). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Memoization cache for already computed coefficients                 */
    /* ------------------------------------------------------------------ */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t cachedlen = 0;

    /* ------------------------------------------------------------------ */
    /* Continued‑fraction state variables (valid after last cached term)   */
    /* ------------------------------------------------------------------ */
    static idx_t  m = 0;           /* previous m value   */
    static idx_t  d = 1;           /* previous d value   */
    static idx_t  a_prev_idx = 29; /* previous coefficient as idx_t (value fits in 64 bits) */

    /* ------------------------------------------------------------------ */
    /* Constant data                                                   */
    /* ------------------------------------------------------------------ */
    const idx_t  S   = 886;        /* radicand of sqrt */
    const idx_t  a0  = 29;         /* floor(sqrt(886)) */

    /* ------------------------------------------------------------------ */
    /* One‑time allocation of the first cache entry                       */
    /* ------------------------------------------------------------------ */
    if (cachedlen == 0) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        cache[0] = a0;                /* a(0) */
        cachedlen = 1;
    }

    /* ------------------------------------------------------------------ */
    /* Extend the cache up to the requested index                         */
    /* ------------------------------------------------------------------ */
    while (cachedlen <= n) {
        /* Compute the next continued‑fraction parameters                 */
        idx_t  m_next = d * a_prev_idx - m;
        idx_t  d_next = (S - m_next * m_next) / d;
        idx_t  a_next_idx = (a0 + m_next) / d_next;   /* floor division */

        /* Grow cache array if necessary                                 */
        if (cachedlen == cap) {
            size_t newcap = cap * 2;
            seq_t *newp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!newp) return (seq_t)0;  /* allocation failed */
            cache = newp;
            cap = newcap;
        }

        /* Store the new coefficient                                     */
        cache[cachedlen] = (seq_t)a_next_idx;

        /* Advance state for the next term                               */
        m          = m_next;
        d          = d_next;
        a_prev_idx = a_next_idx;

        ++cachedlen;
    }

    return cache[(size_t)n];
}
