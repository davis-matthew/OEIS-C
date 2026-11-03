/*
 * A041797  Denominators of continued fraction convergents to sqrt(419).
 * https://oeis.org/A041797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 191 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A041797: Denominators of continued fraction convergents to sqrt(419).
   Recurrence: a(n) = 540349940*a(n-18) - a(n-36) for n>35
   Initial terms for 0 ≤ n < 36 are obtained from the continued‑fraction
   expansion of sqrt(419). The first 36 values are computed directly.
   The function supports arbitrary order of calls by memoizing the
   computed values.
*/

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the first 36 terms directly from the continued fraction of sqrt(419). */
static void compute_initial_36(seq_t *out)
{
    const idx_t D   = 419;          /* Squarefree part */
    const idx_t a0  = 20;           /* floor(sqrt(419)) */

    idx_t m   = 0;
    idx_t d   = 1;
    idx_t a   = a0;

    /* Convergent numerators/denominators: p(-1)=1, p(0)=a0; q(-1)=0, q(0)=1. */
    seq_t p_prevprev = 1;
    seq_t p_prev     = (seq_t)a0;
    seq_t q_prevprev = 0;
    seq_t q_prev     = 1;

    out[0] = 1;                      /* a(0) */

    for (idx_t k = 1; k <= 35; ++k) {
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        seq_t p_next = (seq_t)a_next * p_prev + p_prevprev;
        seq_t q_next = (seq_t)a_next * q_prev + q_prevprev;

        out[k] = q_next;            /* a(k) = denominator of the k‑th convergent */

        m = m_next;
        d = d_next;
        a = a_next;

        p_prevprev = p_prev;
        p_prev = p_next;
        q_prevprev = q_prev;
        q_prev = q_next;
    }
}

/* Global cache for memoization. */
static seq_t *cache   = NULL;   /* array holding a(n) values */
static size_t cache_len = 0;    /* number of cached terms (max index + 1) */

seq_t a(idx_t n)
{
    /* Allocate or enlarge the cache so that it can hold a(n). */
    if (!cache) {
        /* At least 36 entries are needed for the recurrence initialization. */
        size_t alloc = n + 1;
        if (alloc < 36) alloc = 36;
        cache = mem_new(seq_t, alloc);
        if (!cache) return (seq_t)0;    /* allocation failed */
        cache_len = 0;
    } else if (cache_len <= n) {
        size_t alloc = n + 1;
        if (alloc < 36) alloc = 36;
        seq_t *tmp = mem_resize(seq_t, cache, alloc);
        if (!tmp) return (seq_t)0;    /* allocation failed */
        cache = tmp;
    }

    /* Compute the first 36 terms if they are not yet cached. */
    if (cache_len == 0) {
        compute_initial_36(cache);
        cache_len = 36;
    }

    /* If the requested index is already cached, return it directly. */
    if (n < cache_len) return cache[n];

    /* Use the recurrence to fill in the cache up to index n. */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val = (seq_t)540349940 * cache[k - 18] - cache[k - 36];
        cache[k] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
