/*
 * A008978  a(n) = (5*n)!/(n!)^5.
 * https://oeis.org/A008978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)!/(n!)^5
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Exact multiplication and division:  a * num / den.
   Works for all seq_t (built‑in integral or arbitrary precision). */
static inline seq_t mul_div(seq_t a, seq_t num, seq_t den)
{
    return (a * num) / den;
}

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = 1;                        /* a(0) = 1 */
        max_cached = 0;
    }

    /* Expand cache if needed */
    if (n > max_cached) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failed */
        cache = tmp;

        /* Compute values from max_cached+1 up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];
            /* Multiply by the five consecutive integers and
               divide by i each time (exact division). */
            for (int t = 1; t <= 5; ++t) {
                seq_t mult = (seq_t)(5 * i - 5 + t);
                val = mul_div(val, mult, (seq_t)i);
            }
            cache[i] = val;
        }
        max_cached = n;
    }

    return cache[n];
}
