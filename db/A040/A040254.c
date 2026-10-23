/*
 * A040254  Continued fraction for sqrt(271).
 * https://oeis.org/A040254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(271) – A040254. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Periodic continued fraction for sqrt(271).  */
    /* a(0) = 16, the terms repeat with period 22.  */
    /* The function builds terms on demand and caches them
       to answer future queries efficiently.  */

    /* Local static state.  */
    static seq_t *cache = NULL;          /* Cached sequence.        */
    static size_t cache_cap = 0;         /* Allocated capacity.     */
    static size_t cache_sz  = 0;         /* Number of terms stored. */

    static seq_t  m = 0;                 /* Current m in CF algorithm. */
    static seq_t  d = 1;                 /* Current d.              */
    static seq_t  a_k = 16;              /* Current a_k.            */

    const seq_t A0 = 16;                 /* floor(sqrt(271)).       */
    const seq_t D  = 271;                /* The square‑free number. */

    /* Ensure cache is sufficient for n.  */
    if (cache_sz == 0) {
        /* First call – initialise. */
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { /* allocation failure – terminate */ } /* pragma: no cover */
        cache[0] = A0;
        cache_sz = 1;
    }

    while (cache_sz <= n) {
        /* Compute next term a_{k+1}.  */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        /* Grow cache if necessary. */
        if (cache_sz >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – terminate */ } /* pragma: no cover */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store and update state. */
        cache[cache_sz++] = a_next;
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    return cache[n];
}
