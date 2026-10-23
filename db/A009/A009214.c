/*
 * A009214  E.g.f. exp(x*sin(x)) (even powers only).
 * https://oeis.org/A009214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2 * Σ_{k=1..n} (-1)^{k+1} · binomial(2n-1,2k-1) · k ·
 *            a(n-k) , a(0)=1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Helper: multiplicative binomial coefficient C(n,k) in seq_t */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Static cache for memoisation of a(0)..a(max_requested) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache for a(0) */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;     /* a(0) = 1 */
        cache_len = 1;
    }

    /* Grow cache if required */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        /* Compute terms sequentially */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) continue;   /* already set */

            seq_t sum = 0;

            /* iterate over k = 1..i */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t c = binom(2 * i - 1, 2 * k - 1); /* C(2i-1,2k-1) */
                seq_t term = c * (seq_t)k * cache[i - k];
                if ((k & 1) == 0)          /* even k -> negative sign */
                    term = -term;
                sum += term;
            }
            cache[i] = sum * 2;
        }

        cache_len = new_len;
    }

    return cache[n];
}
