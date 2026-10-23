/*
 * A041294  Numerators of continued fraction convergents to sqrt(160).
 * https://oeis.org/A041294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   a(n) = 1442*a(n-8) - a(n-16) (n ≥ 16) a(0..15) are the
 *            numerators of the continued‑fraction convergents to sqrt(160).
 *            They are generated from the standard convergent recurrence for
 *            sqrt(160) and then used as the seed for the above linear
 *            recurrence
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t init[16];
static int init_done = 0;

/* Compute the first 16 terms from the continued fraction of sqrt(160). */
static void compute_initial_terms(void)
{
    const idx_t D = 160;          /* Integer under the square root. */
    idx_t a0 = 12;                /* floor(sqrt(160)). */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    seq_t p_prev2 = 0;   /* p[-2] */
    seq_t p_prev1 = 1;   /* p[-1] */

    for (idx_t k = 0; k < 16; ++k) {
        seq_t p_curr = (seq_t)a * p_prev1 + p_prev2;
        init[k] = p_curr;

        /* Shift p values for next iteration. */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;

        /* Next terms of the continued fraction. */
        idx_t m_next  = d * a - m;
        idx_t d_next  = (D - m_next * m_next) / d;
        idx_t a_next  = (a0 + m_next) / d_next;  /* floor */

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

/* Cache for computed values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t max_computed = (size_t)-1;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!init_done) {
        compute_initial_terms();
        init_done = 1;
    }

    if ((idx_t)n < 16)            /* Initial terms directly. */
        return init[(size_t)n];

    /* Ensure cache has initial 16 values. */
    if (cache == NULL) {
        cache_size   = 16;
        cache        = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)   return 0;           /* Allocation failure. */
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init[i];
        max_computed = 15;
    }

    /* Expand cache if necessary. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 16;
        if ((size_t)n + 1 > new_size)
            new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)   return 0;           /* Allocation failure. */
        cache      = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n. */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)1442 * cache[i - 8] - cache[i - 16];
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}
