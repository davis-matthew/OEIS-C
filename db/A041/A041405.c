/*
 * A041405  Denominators of continued fraction convergents to sqrt(217).
 * https://oeis.org/A041405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 7688126*a(n-16) - a(n-32) for n>31, with initial values
 *            a(0) … a(31) given by the convergents of sqrt(217)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  External integer types are supplied by the build system. */
#include <stddef.h>
#include <stdlib.h>

/* Cached sequence values and bookkeeping */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_cap = 0;     /* allocated capacity (in terms) */
static idx_t cache_filled = 0;   /* highest index that has been computed */

/* Allocate or grow the cache to hold at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_cap > n) return;          /* already large enough */

    size_t new_cap = (size_t)n + 1;
    if (new_cap < 32) new_cap = 32;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;                    /* allocation failed – leave cache unchanged */

    /* Copy already computed values */
    for (idx_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];

    if (cache) free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate the first 32 terms from the continued‑fraction of sqrt(217) */
static void init_terms(void)
{
    if (cache) return;                          /* already initialised */

    cache_cap = 32;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;                          /* allocation failed */

    /* Variables for the continued‑fraction algorithm */
    const idx_t D   = 217;                      /* the square‑free integer */
    const idx_t a0  = 14;                      /* floor(sqrt(217)) */
    idx_t m = 0, d = 1, a = a0;                /* m, d, a for next step */

    seq_t q_prevprev = 1;  /* q_-2 = 1      */
    seq_t q_prev = 0;      /* q_-1 = 0      */

    for (idx_t i = 0; i < 32; ++i) {
        /* Compute the denominator q_i of the i‑th convergent */
        seq_t q = a * q_prev + q_prevprev;
        cache[i] = q;

        /* Shift for next iteration of the recurrence */
        q_prevprev = q_prev;
        q_prev = q;

        /* Compute next continued‑fraction coefficient */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Update state */
        m = m_next;   d = d_next;   a = a_next;
    }

    cache_filled = 31;            /* terms 0..31 have been computed */
}

/* Main entry point for the sequence */
seq_t a(idx_t n)
{
    init_terms();                /* ensure first 32 terms are ready */

    if (n <= cache_filled)      /* already computed value */
        return cache[n];

    ensure_cache(n);             /* enlarge cache if necessary */

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_filled + 1; i <= n; ++i) {
        seq_t term = cache[i - 16] * 7688126;
        term -= cache[i - 32];
        cache[i] = term;
    }

    cache_filled = n;
    return cache[n];
}
