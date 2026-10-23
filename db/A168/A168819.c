/*
 * A168819  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
 *   G(t) = (t^19 + 2*t^18 + 2*t^17 + … + 2*t + 1) /
 *          (1 - 44*(t + t^2 + … + t^18) + 990*t^19)
 *
 * For n ≥ 0:
 *   a(0) = 1
 *   1 ≤ n ≤ 18:   a(n) = 44 * sum_{k=1}^{n}   a(n-k)   + 2
 *   n = 19:       a(19) = 44 * sum_{k=1}^{18} a(19-k) - 990*a(0) + 1
 *   n ≥ 20:       a(n) = 44 * sum_{k=1}^{18} a(n-k)   - 990*a(n-19)
 *
 * The above is a linear recurrence with constant coefficients, suitable
 * for on‑demand memoisation.  The function below stores computed terms
 * in a static cache and expands it as necessary.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Expand cache to include a(n) if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();            /* memory exhausted */
        cache = new_cache;

        /* Compute all new terms up to n */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = 1;               /* a(0) */
            } else if (i <= 18) {            /* 1 ≤ i ≤ 18 */
                seq_t sum = 0;
                for (size_t k = 1; k <= i; ++k)
                    sum += cache[i - k];
                cache[i] = 44 * sum + 2;
            } else if (i == 19) {            /* i = 19 */
                seq_t sum = 0;
                for (size_t k = 1; k <= 18; ++k)
                    sum += cache[19 - k];
                cache[i] = 44 * sum - 990 * cache[0] + 1;
            } else {                         /* i ≥ 20 */
                seq_t sum = 0;
                for (size_t k = 1; k <= 18; ++k)
                    sum += cache[i - k];
                cache[i] = 44 * sum - 990 * cache[i - 19];
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
