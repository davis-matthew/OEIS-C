/*
 * A170195  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170195
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

/* Recurrence: 
   a(0) = 1,  for 1 ≤ k ≤ 38: a(k) = 2 + 40*∑_{i=1}^{k} a(k-i) (derived from the numerator),
   for k ≥ 39: a(k) = 40*∑_{i=1}^{38} a(k-i) – 820*a(k-39) .
   The first terms are:
   a(0)=1, a(1)=42, a(2)=1722, a(3)=70602, a(4)=2894682, …  (see OEIS A170195). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored (next missing index = cache_size) */
    static size_t cache_cap  = 0;   /* allocated capacity */

    size_t target = (size_t)n;

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 64; /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed */
        cache_size = 0;
    }

    /* grow the cache if necessary */
    if (target >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= target) new_cap <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to target */
    for (size_t k = cache_size; k <= target; ++k) {
        seq_t summ; /* intermediate value */

        /* constant term from the numerator */
        if (k == 0 || k == 39)
            summ = 1;
        else if (k <= 38)
            summ = 2;
        else
            summ = 0;

        /* add 40 * a(k-i) for i = 1 .. min(k,38) */
        size_t add_end = (k < 39) ? k : 38; /* highest i to add */
        for (size_t i = 1; i <= add_end; ++i) {
            summ += (seq_t)40 * cache[k - i];
        }

        /* subtract 820 * a(k-39) if k >= 39 */
        if (k >= 39) {
            summ -= (seq_t)820 * cache[k - 39];
        }

        cache[k] = summ;
    }

    cache_size = target + 1;
    return cache[n];
}
