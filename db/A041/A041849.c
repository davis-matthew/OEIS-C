/*
 * A041849  Denominators of continued fraction convergents to sqrt(446).
 * https://oeis.org/A041849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 8, and for n ≥ 2 a(n) = coeff[n‑1] * a(n‑1) +
 *            a(n‑2), where coeff array contains the continued‑fraction
 *            coefficients of √446, namely 8,2,2,1,3,1,1,20,1,1,3,1,2,2,8,42
 *            repeating
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Period of the continued fraction for sqrt(446) */
#define PERIOD_LEN 16
static const idx_t coeff[PERIOD_LEN] = {
    8, 2, 2, 1, 3, 1, 1, 20,
    1, 1, 3, 1, 2, 2, 8, 42
};

/* Cache to store already computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* allocated capacity */
static idx_t max_cached = 0;   /* largest n for which cache[n] is valid */

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Lazy‑initialise the cache with the first 2 terms. */
    if (cache == NULL) {
        cache_size = (n >= 1) ? n + 1 : 2;
        if (cache_size < 2) cache_size = 2;                     /* at least two slots */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failed; should not happen */
        cache[0] = (seq_t)1;                    /* a(0) = 1 */
        if (cache_size > 1)
            cache[1] = coeff[0] * cache[0];     /* a(1) = coeff[0] * a(0) */
        max_cached = 1;
    }

    /* Resize cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;        /* allocation failed; unlikely */
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        idx_t idx = (k - 1) % PERIOD_LEN;
        seq_t c = (seq_t)coeff[idx];
        seq_t val;
        if (k == 1) {
            val = c * cache[0];
        } else {
            val = c * cache[k - 1] + cache[k - 2];
        }
        cache[k] = val;
    }
    max_cached = n;

    return cache[n];
}
