/*
 * A169429  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30⋅∑_{k=1}^{31} a(n‑k) – 465⋅a(n‑32) + p(n) , where p(n)
 *            = 1 if n=0 or 32, p(n)=2 for 1≤n≤31, and p(n)=0 otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache of already computed terms */
    static size_t cache_size = 0;   /* number of terms stored in cache */

    /* Ensure the cache holds terms up to index n */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);

        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (cache == NULL) abort();
        } else if (new_size > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
        }

        /* Compute missing terms */
        size_t start = cache_size;        /* first index not yet computed */
        for (idx_t i = (idx_t)start; i <= n; ++i) {
            seq_t sum = 0;

            /* 30 * Σ a(i-k) for k = 1..31 (only available terms) */
            idx_t max_k = (i < 31) ? i : 31;
            for (idx_t k = 1; k <= max_k; ++k) {
                sum += cache[i - k] * 30;
            }

            /* subtract 465 * a(i-32) if available */
            if (i >= 32) {
                sum -= cache[i - 32] * 465;
            }

            /* add the numerator contribution p(i) */
            seq_t p = 0;
            if (i == 0)               p = 1;
            else if (i <= 31)         p = 2;
            else if (i == 32)         p = 1;
            /* else p remains 0 for i > 32 */

            cache[i] = sum + p;
        }

        cache_size = new_size;
    }

    return cache[n];
}
