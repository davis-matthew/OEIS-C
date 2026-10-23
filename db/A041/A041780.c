/*
 * A041780  Numerators of continued fraction convergents to sqrt(411).
 * https://oeis.org/A041780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, less
 * Formula:   a(n) = numerator of the nth convergent to √411.
 *            Continued‑fraction coefficients: a0 = 20, (a1,…,a10) =
 *            {3,1,1,1,19,1,1,1,3,40} repeating. The numerators satisfy p[n] =
 *            a[n]·p[n−1] + p[n−2] (p[−1]=1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_cache = NULL;      /* cache of computed numerators */
static idx_t cache_size = 0;       /* current length of cache (number of valid entries) */

/* Return the nth continued‑fraction coefficient of √411. */
static seq_t cf_coeff(idx_t idx)
{
    if (idx == 0u)   /* first term */
        return (seq_t)20;
    /* period of length 10 after the initial term */
    static const seq_t period[10] = {3,1,1,1,19,1,1,1,3,40};
    return period[(idx - 1u) % 10u];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure the cache contains the first two terms */
    if (cache_size == 0u) {
        cache_size = 2u;                      /* indices 0 and 1 known */
        p_cache = (seq_t *)malloc(2u * sizeof(seq_t));
        if (!p_cache)                         /* allocation failure */
            return (seq_t)0;
        p_cache[0u] = (seq_t)20;
        p_cache[1u] = (seq_t)61;
    }

    if (n < cache_size)
        return p_cache[n];

    /* Grow the cache to accommodate index n */
    seq_t *new_cache = (seq_t *)realloc(p_cache, (n + 1u) * sizeof(seq_t));
    if (!new_cache)                           /* allocation failure */
        return (seq_t)0;
    p_cache = new_cache;

    /* Compute terms from current cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t coeff = cf_coeff(i);
        /* p[i] = coeff * p[i-1] + p[i-2] */
        p_cache[i] = coeff * p_cache[i - 1u] + p_cache[i - 2u];
    }

    cache_size = n + 1u;
    return p_cache[n];
}
