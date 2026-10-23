/*
 * A137360  a(n) = Sum_{k <= n/2 } k*binomial(n-2k, 3k+1).
 * https://oeis.org/A137360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k <= n/2} k * binomial(n-2k, 3k+1)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */
#include <stdlib.h>            /* for malloc, realloc */
#include "../utils/binomial.h" /* for binomial(idx_t, idx_t) */

/* Compute a single term a(n) directly using the definition. */
static seq_t compute_a(idx_t n)
{
    seq_t sum = 0;
    idx_t k_max = n / 2;            /* integer division is fine for idx_t */
    for (idx_t k = 0; k <= k_max; ++k) {
        idx_t n1 = n - 2 * k;
        idx_t k1 = 3 * k + 1;
        if (k1 > n1)                /* binomial would be zero */
            continue;
        seq_t val = binomial(n1, k1);    /* binomial returns seq_t */
        val *= (seq_t)k;               /* multiply by k */
        sum += val;
    }
    return sum;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values so that unordered calls are fast. */
    static seq_t *cache = NULL;      /* array of values a(0), a(1), … */
    static size_t cap = 0;           /* allocated size of cache */
    static idx_t max_computed = 0;   /* highest n that has been computed so far */

    /* Allocate or enlarge the cache if necessary. */
    if (n >= (idx_t)cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t t = max_computed + 1; t <= n; ++t)
        cache[t] = compute_a(t);
    max_computed = (n > max_computed) ? n : max_computed;

    return cache[n];
}
