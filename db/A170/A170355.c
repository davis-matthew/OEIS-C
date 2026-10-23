/*
 * A170355  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=10... with recurrence * a(n) = 8*∑_{i=0}^{n-1} a(i)
 *            + 2 for 1≤n≤42 * a(43) = 8*∑_{i=1}^{42} a(i) – 36*a(0) + 1 *
 *            a(n) = 8*∑_{i=n-42}^{n-1} a(i) – 36*a(n-43) for n≥44 * (derived
 *            from the rational generating function in the OEIS entry). * The
 *            function memoises results so any order of c...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* enlarge cache if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t val;
        if (k == 0) {
            val = (seq_t)1;
        } else if (k >= 1 && k <= 42) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i < k; ++i) sum += cache[i];
            val = 8 * sum + (seq_t)2;
        } else if (k == 43) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= 42; ++i) sum += cache[i];
            val = 8 * sum - 36 * cache[0] + (seq_t)1;
        } else { /* k >= 44 */
            seq_t sum = (seq_t)0;
            for (idx_t i = k - 42; i < k; ++i) sum += cache[i];
            val = 8 * sum - 36 * cache[k - 43];
        }
        cache[k] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
