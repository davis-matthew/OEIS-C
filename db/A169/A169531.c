/*
 * A169531  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=36*Σ_{k=1}^{min(33,n)} a(n−k) −
 *            666*a(n−34) + p_n, where p_0=1, p_1…p_33=2, p_34=1, others 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build (not defined here). */

static seq_t *cache = NULL;     /* array of computed a(n) */
static size_t cache_len = 0;    /* number of values currently stored */
static size_t cache_cap = 0;    /* allocated size of cache */

/* Return the numerator coefficient p_n of the generating function. */
static inline seq_t p_value(size_t n)
{
    if (n == 0) return (seq_t)1;
    if (n <= 33) return (seq_t)2;
    if (n == 34) return (seq_t)1;
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Ensure the first value a(0)=1 is present. */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Grow the cache if needed and compute missing terms. */
    if (idx >= cache_len) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= idx) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;

        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t sum = (seq_t)0;
            size_t maxk = (i >= 33) ? 33 : i;
            for (size_t k = 1; k <= maxk; ++k)
                sum += cache[i - k];

            seq_t val = sum * (seq_t)36 + p_value(i);
            if (i >= 34)
                val -= cache[i - 34] * (seq_t)666;

            cache[i] = val;
        }

        cache_len = idx + 1;
    }

    return cache[idx];
}
