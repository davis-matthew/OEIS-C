/*
 * A042412  Numerators of continued fraction convergents to sqrt(734).
 * https://oeis.org/A042412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 12, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for convergents of sqrt(734): a(0)=27,
 * for n≥1   a(n)=a_k·a(n‑1)+a(n‑2) where
 * a_k, m_k, d_k are successive partial quotients of
 * the continued fraction of sqrt(734).  This function
 * returns the numerator of the n‑th convergent. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constant data for sqrt(734). */
    static const idx_t D   = 734;
    static const idx_t a0i = 27;                 /* floor(sqrt(D)) */
    static const seq_t seq_a0 = (seq_t)27;        /* a0 as seq_t */

    /* Cache storage for already‑computed numerators. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;                 /* number of terms stored */

    /* State of the continued‑fraction machinery. */
    static idx_t m      = 0;                     /* m_{k‑1} */
    static idx_t d      = 1;                     /* d_{k‑1} */
    static idx_t a_cur  = a0i;                   /* a_k for next step */

    /* Storage of previous two convergents. */
    static seq_t p_prevprev = (seq_t)1;          /* p_{‑1} */
    static seq_t p_prev     = seq_a0;            /* p_0   */

    /* First‑time allocation of the cache. */
    if (!cache) {
        cache_cap = 16;                          /* initial capacity */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Initialise the first element if we haven't yet. */
    if (cache_len == 0) {
        cache[0] = seq_a0;      /* a(0) = 27 */
        cache_len = 1;
    }

    /* Extend the cache up to the required index n. */
    if ((idx_t)cache_len <= n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* Next partial quotient a_k, m_k, d_k (k ≥ 1). */
            idx_t next_m = d * a_cur - m;
            idx_t next_d = (D - next_m * next_m) / d;
            idx_t next_a = (a0i + next_m) / next_d;

            /* Numerator of the k‑th convergent. */
            seq_t next_p = (seq_t)next_a * p_prev + p_prevprev;

            /* Grow the cache if necessary. */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = next_p;

            /* Advance the continued‑fraction state. */
            m = next_m;
            d = next_d;
            a_cur = next_a;

            /* Shift the convergent history. */
            p_prevprev = p_prev;
            p_prev = next_p;
        }
    }

    return cache[(size_t)n];
}
