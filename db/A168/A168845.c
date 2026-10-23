/*
 * A168845  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * for 1 ≤ n ≤ 19: a(n) = 22·∑_{i=0}^{n-1} a(i) + 2 *
 *            for n ≥ 20: a(n) = 22·∑_{i=n-19}^{\,n-1} a(i) – 253·a(n‑20) * *
 *            This recurrence follows from the rational generating function *
 *            (t^20+2t^19+⋯+2t+1)/(1–22t–22t^2–⋯–22t^19+253t^20) . * The
 *            function memoises previously computed terms for...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache    = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_size = 0;

    /* grow cache as needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    while (cache_size <= (size_t)n) {
        /* index of the term to compute */
        size_t idx = cache_size;

        if (idx == 0) {
            cache[0] = (seq_t)1;
        } else if (idx <= 19) {
            /* a(idx) = 22 · sum_{i=0}^{idx-1} a(i) + 2   (idx = 1..19) */
            seq_t sum = (seq_t)0;
            for (size_t j = 0; j < idx; ++j) sum += cache[j];
            cache[idx] = (seq_t)22 * sum + (seq_t)2;
        } else if (idx == 20) {
            /* a(20) = 1 + 22 · sum_{i=1}^{19} a(i) – 253·a(0)   */
            seq_t sum19 = (seq_t)0;
            for (size_t j = 1; j <= 19; ++j) sum19 += cache[j];
            cache[20] = (seq_t)1 + (seq_t)22 * sum19 - (seq_t)253 * cache[0];
        } else {
            /* a(idx) = 22 · ∑_{i=idx-19}^{idx-1} a(i) – 253·a(idx-20)  */
            seq_t sum19 = (seq_t)0;
            for (size_t j = idx - 19; j < idx; ++j) sum19 += cache[j];
            cache[idx] = (seq_t)22 * sum19 - (seq_t)253 * cache[idx - 20];
        }

        ++cache_size;
    }

    return cache[n];
}
