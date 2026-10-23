/*
 * A170171  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 18. For 2 ≤ n ≤ 38: a(n) = 2 + 16 ·
 *            Σ_{k=0}^{n-1} a(k). For n ≥ 39: a(n) = 16 · Σ_{k=1}^{38} a(n‑k)
 *            – 136 · a(n‑39)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;   /* array of sequence values   */
    static size_t capacity = 0;      /* allocated size (in elements) */
    static size_t computed = 0;      /* number of elements already computed */

    /* enlarge cache if necessary (ensure at least 39 slots) */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 39) new_cap = 39;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out of memory – return 0 */
        for (size_t i = 0; i < capacity; ++i) new_cache[i] = cache[i];
        free(cache);
        cache    = new_cache;
        capacity = new_cap;
    }

    /* initialise first 39 terms once */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        seq_t cumulative = cache[0];
        for (idx_t i = 1; i < (idx_t)39; ++i) {
            cache[i] = (seq_t)2 + (seq_t)16 * cumulative;
            cumulative += cache[i];
        }
        computed = 39;
    }

    /* if the requested term is already known, return it */
    if ((size_t)n < computed) return cache[n];

    /* compute any missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum38 = (seq_t)0;
        for (idx_t j = 1; j <= 38; ++j) {
            sum38 += cache[i - j];
        }
        cache[i] = (seq_t)16 * sum38 - (seq_t)136 * cache[i - 39];
    }
    computed = (size_t)n + 1;
    return cache[n];
}
