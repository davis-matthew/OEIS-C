/*
 * A220893  G.f.: 1/H(-x), where H(x) = (1+8*x+22*x^2+8*x^3+x^4)/(1-x)^6.
 * https://oeis.org/A220893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 8*a(n-1) - 22*a(n-2) + 8*a(n-3) - a(n-4) + C(6,n) for n ≥
 *            0, where a(k)=0 for k<0 and a(0)=1
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */

seq_t a(idx_t n)
{
    /* first call: set up cache */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first term */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* binomial coefficients C(6,i) for i = 0..6 */
    static const seq_t binom[7] = {1, 6, 15, 20, 15, 6, 1};

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = 0;
        if (i >= 1) val += 8 * cache[i-1];
        if (i >= 2) val -= 22 * cache[i-2];
        if (i >= 3) val += 8 * cache[i-3];
        if (i >= 4) val -= cache[i-4];
        if (i <= 6) val += binom[i];
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
