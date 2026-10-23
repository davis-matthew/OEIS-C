/*
 * A042568  Numerators of continued fraction convergents to sqrt(813).
 * https://oeis.org/A042568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 4334*a(n-6) – a(n-12) for n ≥ 12; a(0)…a(11) are the
 *            numerators of the convergents to sqrt(813). This implementation
 *            generates the first 12 terms by a standard continued‑fraction
 *            algorithm and then applies the linear recurrence, caching
 *            results for arbitrary (unordered) calls
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root for a 64‑bit unsigned integer. */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* Generate the first 12 numerators of the continued fraction for sqrt(813). */
static void init_sequence(seq_t init[12])
{
    const idx_t D   = 813UL;                   /* discriminant */
    idx_t a0        = int_sqrt(D);            /* floor(sqrt(813)) = 28 */
    idx_t m         = 0;
    idx_t d         = 1;
    idx_t a         = a0;

    seq_t p_prev2   = 0;   /* p(-2)  */
    seq_t p_prev1   = 1;   /* p(-1)  */

    for (idx_t k = 0; k < 12; ++k) {
        /* p(k) = a*p(-1) + p(-2) */
        seq_t p = (seq_t)( (seq_t)a * p_prev1 + p_prev2 );
        init[(size_t)k] = p;

        /* next iteration of the quadratic‑irrational algorithm   */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        p_prev2 = p_prev1;
        p_prev1 = p;
    }
}

/* Main entry point for the sequence. */
seq_t a(idx_t n)
{
    /* Static storage for the first 12 values. */
    static seq_t init[12];
    static int init_done = 0;
    if (!init_done) {
        init_sequence(init);
        init_done = 1;
    }

    if (n < 12)
        return init[(size_t)n];

    /* Static cache for terms ≥12. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;

    /* Initial allocation and seed with the first 12 terms. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();   /* allocation failure */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_size = 12;
    }

    /* Grow the cache if necessary. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Use 64‑bit arithmetic to avoid overflow in the product. */
        seq_t val = (seq_t)((uint64_t)4334 * cache[i - 6] - cache[i - 12]);
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[(size_t)n];
}
