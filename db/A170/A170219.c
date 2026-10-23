/*
 * A170219  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 0, a(n) = 16 * (a(n-1) + a(n-2) + … + a(n-39))
 *            – 136 * a(n-40) + N(n), where N(n) = 1 (n = 0 or 40), 2 (1 ≤ n ≤
 *            39), 0 (n > 40). (Undefined indices a(k) for k < 0 are treated
 *            as 0.) This recurrence generates OEIS A170219
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 0;   /* highest index computed so far */

    /* ensure cache is large enough */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – program cannot continue safely */
            abort();
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise seed for n = 0 if not done */
    if (last_computed == 0 && cache[0] == 0) {
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* add 16 * a(i - k) for 1 ≤ k ≤ 39 */
        for (idx_t k = 1; k <= 39; ++k) {
            if (i >= k) {
                sum += cache[i - k] * (seq_t)16;
            }
        }

        /* subtract 136 * a(i - 40) if i ≥ 40 */
        if (i >= 40) {
            sum -= cache[i - 40] * (seq_t)136;
        }

        /* add N(i) */
        if (i == 0 || i == 40) {
            sum += (seq_t)1;
        } else if (i < 40) {
            sum += (seq_t)2;
        }
        /* for i > 40, N(i) = 0 – nothing to add */

        cache[i] = sum;
    }

    last_computed = n;
    return cache[n];
}
