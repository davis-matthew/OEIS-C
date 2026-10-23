/*
 * A175981  a(1) = 1, for n >= 2: a(n) = a(n-1)^(n-1) - 1.
 * https://oeis.org/A175981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n >= 2: a(n) = a(n-1)^(n-1) - 1
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 6, i64 n <= 6, i128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;      /* cache[1] .. cache[n] */
static idx_t cache_size = 0;     /* allocated slots (≥ n+1) */
static idx_t last_computed = 0;  /* highest index already stored */

/* Return a(n) for any n >= 1, using memoization. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, guard */

    /* Ensure cache is initialized. */
    if (cache == NULL) {
        cache_size = (n >= 2) ? n + 1 : 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;          /* unused sentinel */
        cache[1] = (seq_t)1;          /* a(1) = 1 */
        last_computed = 1;
    }

    /* Grow cache if necessary. */
    if (cache_size <= n) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Compute missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t exp  = (seq_t)(i - 1);   /* exponent (i-1) */
        seq_t val  = big_pow(prev, exp);
        val -= (seq_t)1;               /* subtract 1 */
        cache[i] = val;
    }
    last_computed = n;

    return cache[n];
}
