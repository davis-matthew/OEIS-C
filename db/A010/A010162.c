/*
 * A010162  Continued fraction for sqrt(91).
 * https://oeis.org/A010162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 9399, u16 n <= 9399, u32 n <= 9399, u64 n <= 9399,
 *            u128 n <= 9399, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction expansion for √91 */
/* The sequence a(n) gives the simple continued‑fraction partial quotients
   of √91.  The recurrence is:
        m0 = 0,   d0 = 1,   a0 = ⌊√91⌋ = 9
        m(k+1) = d(k)·a(k) − m(k)
        d(k+1) = (91 − m(k+1)²) / d(k)
        a(k+1) = ⌊(a0 + m(k+1)) / d(k+1)⌋
   The integers a(k) are indexed from 0.  The function memoises values so
   that arbitrary‐order calls are fast. */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of cached terms (always ≥ 1 after init) */

    /* state of the continued‑fraction algorithm */
    static bool initialized = false;
    static seq_t m_cur = 0;                /* m_k   */
    static seq_t d_cur = 1;                /* d_k   */
    static seq_t a_cur = 9;                /* a_k   */
    const seq_t A0 = 9;                    /* a0 = ⌊√91⌋ */
    const seq_t S  = 91;                   /* value whose root we expand */

    /* initialise on first call */
    if (!initialized) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = a_cur;                 /* a(0) = 9 */
        initialized = true;
    }

    /* if already computed return cached value */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* generate terms up to n */
    while (cache_len <= (size_t)n) {
        /* m(k+1) = d(k)·a(k) − m(k) */
        seq_t m_next = d_cur * a_cur - m_cur;

        /* d(k+1) = (S − m(k+1)²) / d(k) */
        seq_t m_next_sq = m_next * m_next;
        seq_t d_next = (S - m_next_sq) / d_cur;

        /* a(k+1) = ⌊(a0 + m(k+1)) / d(k+1)⌋ */
        seq_t a_next = (A0 + m_next) / d_next;

        /* extend cache */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_len + 1));
        if (!tmp) {
            /* allocation failure – very unlikely for the given limits */
            abort();
        }
        cache = tmp;
        cache[cache_len] = a_next;
        ++cache_len;

        /* advance state */
        m_cur = m_next;
        d_cur = d_next;
        a_cur = a_next;
    }

    return cache[(size_t)n];
}
