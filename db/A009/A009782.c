/*
 * A009782  E.g.f.: expansion of tanh(log(1+x))/exp(x).
 * https://oeis.org/A009782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = series expansion of tanh(log(1+x))/exp(x) Recurrence:
 *            a(0)=0, a(1)=1, and for n≥0 a(n+1) = (-1)^(n+1) (n+1)(n-2)/2 –
 *            (n+1)a(n) – (n+1)n/2·a(n-1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t  cache_computed = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Initialise cache if first call */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_computed = 1;
    }

    /* Grow cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_computed; i < n; ++i) {
        seq_t sign = (((i + 1) & 1ULL) ? (seq_t)(-1) : (seq_t)1);

        /* term1 = (-1)^(i+1) * (i+1)*(i-2)/2 */
        seq_t prod1 = (seq_t)(i + 1) * ((seq_t)i - (seq_t)2);
        seq_t term1 = sign * (prod1 / 2);

        /* term2 = (i+1) * a(i) */
        seq_t term2 = (seq_t)(i + 1) * cache[i];

        /* term3 = (i+1)*i/2 * a(i-1)   (zero if i==0) */
        seq_t term3 = 0;
        if (i >= 1) {
            seq_t prod3 = (seq_t)(i + 1) * (seq_t)i;
            term3 = (prod3 / 2) * cache[i - 1];
        }

        cache[i + 1] = term1 - term2 - term3;
    }

    cache_computed = n;
    return cache[n];
}
