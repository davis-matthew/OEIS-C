/*
 * A168913  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1: let rhs(n) = 1 if n == 0 or n == 21, 2 if 1
 *            ≤ n ≤ 20, 0 otherwise. Then for all n ≥ 1 the recurrence holds:
 *            a(n) = rhs(n) + Σ_{k=1}^{20} 42·a(n-k) - 903·a(n-21) (a(m)=0 for
 *            m<0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* External types are supplied by the build system: seq_t, idx_t */

/* Memoized cache for arbitrary call order */
static seq_t *cache        = NULL;
static idx_t cache_len     = 0;
static idx_t cache_cap     = 0;

seq_t a(idx_t n)
{
    /* Ensure storage for indices up to n */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = seq_alloc_array((size_t)cache_cap);
        cache_len = 0;
    } else if (n + 1 > cache_cap) {
        seq_t *new_arr = seq_alloc_array((size_t)(n + 1));
        for (idx_t i = 0; i < cache_cap; ++i) new_arr[i] = cache[i];
        seq_free_array(cache);
        cache = new_arr;
        cache_cap = n + 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t rhs;
        if (i == 0)            rhs = 1;
        else if (i <= 20)      rhs = 2;
        else if (i == 21)      rhs = 1;
        else                   rhs = 0;

        seq_t sum = rhs;
        for (idx_t k = 1; k <= 20; ++k) {
            if (i >= k) sum += cache[i - k] * (seq_t)42;
        }
        if (i >= 21) sum -= cache[i - 21] * (seq_t)903;

        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
