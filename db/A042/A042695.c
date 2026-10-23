/*
 * A042695  Denominators of continued fraction convergents to sqrt(877).
 * https://oeis.org/A042695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   for n > 29: a(n) = 482652*a(n-15) + a(n-30) Initial values 0 <=
 *            n <= 29 are generated from the convergents of sqrt(877)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 29, u64 n <= 53, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a cache, so any order of calls works. */

#include <stdint.h>
#include <stdlib.h>

/* Helper to grow an array: double capacity or set to needed. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap * 2) : 16;
    if (new_cap < needed) new_cap = needed;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* continued‑fraction state for the first 30 terms */
    static const uint64_t SQRT_N = 877ULL;   /* D */
    static const uint64_t a0 = 29;           /* floor(sqrt(877)) */
    static uint64_t m = 0;
    static uint64_t d = 1;
    static uint64_t a_curr = a0;            /* a_k : starts with a0 */

    /* if cache empty, initialise first term */
    if (cache == NULL) {
        ensure_capacity(1, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if (n < cache_len)                /* already computed */
        return cache[n];

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        ensure_capacity(i + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

        if (i < 30) {                 /* generate via continued fraction */
            /* next CF coefficient */
            uint64_t m_next = d * a_curr - m;
            uint64_t d_next = (SQRT_N - m_next * m_next) / d;
            uint64_t a_next = (a0 + m_next) / d_next;

            /* denominator recurrence */
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = (i >= 2) ? cache[i - 2] : (seq_t)0;
            cache[i] = (seq_t)a_next * prev1 + prev2;

            /* advance state for next step */
            m = m_next;
            d = d_next;
            a_curr = a_next;
        } else {                      /* recurrence for i >= 30 */
            cache[i] = ((seq_t)482652) * cache[i - 15] + cache[i - 30];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
