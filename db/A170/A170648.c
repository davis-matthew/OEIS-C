/*
 * A170648  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A170648: number of reduced words of length n
 *  in Coxeter group on 15 generators with relations (S_i)^2 = (S_iS_j)^49 = I.
 *
 *  Recurrence derived from the rational generating function:
 *
 *    a(0) = 1
 *    for 1 ≤ n ≤ 48:
 *      a(n) = 2 + 13 * Σ_{i=0}^{n-1} a(i)
 *    a(49) = 1 + 13 * Σ_{i=1}^{48} a(i) – 91 * a(0)
 *    for n ≥ 50:
 *      a(n) = 13 * Σ_{i=n-48}^{n-1} a(i) – 91 * a(n-49)
 *
 *  The function caches values so that calls in any order are efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored */
    static seq_t pref_sum = 0;    /* sum of all stored terms */

    /* Allocate or grow the cache if needed */
    if (cache == NULL) {
        cache_len = 0;
        pref_sum  = 0;
        size_t init_cap = (size_t)n + 1;
        if (init_cap == 0) init_cap = 1;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed */
    }
    if (cache_len <= n) {
        size_t needed = (size_t)n + 1;
        if (needed > (size_t)cache_len) {
            size_t new_cap = (cache_len == 0) ? needed : cache_len * 2;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* reallocation failed */
            cache = tmp;
        }

        /* Compute terms from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = 1;
            } else if (i <= 48) {
                val = (seq_t)2 + (seq_t)13 * pref_sum;
            } else if (i == 49) {
                seq_t sum_rest = pref_sum - cache[0];
                val = (seq_t)1 + (seq_t)13 * sum_rest - (seq_t)91 * cache[0];
            } else { /* i >= 50 */
                seq_t sum_last_48 = 0;
                for (idx_t j = i - 48; j <= i - 1; ++j) {
                    sum_last_48 += cache[j];
                }
                val = (seq_t)13 * sum_last_48 - (seq_t)91 * cache[i - 49];
            }
            cache[i] = val;
            pref_sum += val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
