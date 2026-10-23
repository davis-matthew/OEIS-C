/*
 * A169070  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the rational generating function given in OEIS
 *            A169070. The function below memoises the sequence using a
 *            recurrence derived from that generating function. It works for
 *            any build where `seq_t` is an unsigned integer type (possibly
 *            big) and `idx_t` is an unsigned 64‑bit integer. The...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache that grows as needed.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Grow the cache if necessary.  */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 32;
        while (new_cap <= n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n, if not already available.  */
    if ((size_t)n + 1 > cache_len) {
        for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
            seq_t p;
            if (k == 0)            p = 1;
            else if (k <= 24U)     p = 2;
            else if (k == 25U)     p = 1;
            else                   p = 0;

            if (k <= 24U) {  /* Sum all previous terms.  */
                seq_t sum = 0;
                for (idx_t i = 0; i < k; ++i) sum += cache[i];
                cache[k] = 7 * sum + p;
            } else {          /* k >= 25.  */
                seq_t sum = 0;
                for (idx_t i = k - 24; i < k; ++i) sum += cache[i];
                cache[k] = 7 * sum - 28 * cache[k - 25] + p;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
