/*
 * A169440  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A169440
    Number of reduced words of length n in the Coxeter group on 43 generators
    with relations S_i^2 = (S_i S_j)^32 = 1.

    Recurrence (derived from its generating function):
        a(0) = 1
        For n ≥ 1:
        a(n) = 41 * Σ_{k=1}^{31} a(n–k) – 861 * a(n–32) + b(n)

    where
        b(n) = 1        if n == 0 or n == 32
              = 2        if 1 ≤ n ≤ 31
              = 0        otherwise
*/

#ifdef __cplusplus
#include <vector>
#endif
#ifndef __cplusplus
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(1);
        cache[0] = 1;
    }

    if (n >= cache.size()) {
        size_t old_size = cache.size();
        cache.resize(n + 1);
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t sum31 = 0;
            for (idx_t k = 1; k <= 31 && i >= k; ++k)
                sum31 += cache[i - k];

            seq_t val = (seq_t)41 * sum31;
            if (i >= 32)
                val -= (seq_t)861 * cache[i - 32];

            seq_t bn = 0;
            if (i == 0)
                bn = 1;
            else if (i <= 31)
                bn = 2;
            else if (i == 32)
                bn = 1;

            cache[i] = val + bn;
        }
    }
    return cache[n];
#else   /* C version */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)
        return 1;

    /* Allocate or enlarge the cache */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache_size = 1;
        cache[0] = 1;
    }

    if (n >= cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
        if (tmp == NULL) {  /* allocation failure – return 0 as a safe fallback */
            free(cache);
            cache = NULL;
            cache_size = 0;
            return 0;
        }
        cache = tmp;

        for (idx_t i = old_size; i <= n; ++i) {
            seq_t sum31 = 0;
            for (idx_t k = 1; k <= 31 && i >= k; ++k)
                sum31 += cache[i - k];

            seq_t val = (seq_t)41 * sum31;
            if (i >= 32)
                val -= (seq_t)861 * cache[i - 32];

            seq_t bn = 0;
            if (i == 0)
                bn = 1;
            else if (i <= 31)
                bn = 2;
            else if (i == 32)
                bn = 1;

            cache[i] = val + bn;
        }
    }
    return cache[n];
#endif
}
