/*
 * A041489  Denominators of continued fraction convergents to sqrt(261).
 * https://oeis.org/A041489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of convergents to sqrt(261) Recurrence (for
 *            the sequence values): a(n) = 384238402 * a(n-16) - a(n-32) (for
 *            n > 31) Initial values: a(0) = 1, a(1) = 6, a(2) = 13,
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 17, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Cache of already computed values */
    static seq_t *cache      = NULL;   /* array of denominators q_k */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  last_comp  = 0;      /* largest n for which cache[n] is valid */
    /* Continued‑fraction state for generating a_k sequence */
    static uint64_t m = 0;   /* m_k   (start value m_0) */
    static uint64_t d = 1;   /* d_k   (start value d_0) */
    static uint64_t a_cf = 16; /* a_k   (start value a_0) */
    static const uint64_t D = 261;      /* D = 261 for sqrt(D) */
    static const uint64_t a0 = 16;      /* floor(sqrt(D)) */
    static bool initialized = false;

    /* Allocate minimal cache on first use */
    if (!initialized) {
        cache_cap = 1;                         /* at least index 0 */
        cache = alloc_seq_array(cache_cap);
        if (!cache) return 0;                  /* out of memory */
        cache[0] = (seq_t)1;                  /* q_0 = 1 */
        last_comp = 0;
        initialized = true;
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1) * 2;   /* grow by factor 2 */
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) return 0;              /* out of memory */
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = last_comp + 1; k <= n; ++k) {
        /* Next continued‑fraction coefficient a_k */
        uint64_t m_next   = d * a_cf - m;
        uint64_t d_next   = (D - m_next * m_next) / d;
        uint64_t a_next   = (a0 + m_next) / d_next;

        /* Convergent denominator recurrence: q_k = a_k*q_{k-1} + q_{k-2} */
        seq_t prev     = cache[k - 1];
        seq_t prevprev = (k >= 2) ? cache[k - 2] : (seq_t)0;
        cache[k] = (seq_t)a_next * prev + prevprev;

        /* Advance continued‑fraction state */
        m = m_next;
        d = d_next;
        a_cf = a_next;
    }
    last_comp = n;

    return cache[n];
}
