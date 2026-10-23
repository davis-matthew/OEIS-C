/*
 * A168687  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 10, a(n) = 9*a(n-1) - 44*a(n-17) + 36*a(n-18)
 *            for n ≥ 2, with an extra -1 added when n = 17 or n = 18 (this
 *            comes from the constant terms in the generating function)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization of already-computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity   */
    static size_t cache_len = 0;   /* number of computed terms  */

    /* target is number of terms needed (indices 0 … n inclusive) */
    size_t target = (size_t)n + 1;
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap < target) new_cap = target;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    if (cache_len < target) {
        for (size_t i = cache_len; i < target; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 10;
            } else {
                seq_t term = 9 * cache[i-1];
                if (i >= 17) term -= 44 * cache[i-17];
                if (i >= 18) term += 36 * cache[i-18];
                if (i == 17 || i == 18) term -= 1;
                cache[i] = term;
            }
        }
        cache_len = target;
    }

    return cache[n];
}
