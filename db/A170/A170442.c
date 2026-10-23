/*
 * A170442  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n≥1 a(n)=c(n)+47*∑_{i=1}^{43} a(n-i)−1128*a(n-44)
 *            where c(n)=2 for 1≤n≤43, c(44)=1, and c(n)=0 for n>44
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;      /* array of computed values    */
    static size_t cache_len = 0;     /* allocated capacity          */
    static size_t cache_max = 0;     /* highest index computed (≤) */

    /* Ensure cache can hold indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        size_t new_len = needed * 2;
        if (new_len < needed) new_len = needed;          /* guard overflow */
        seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return 0; /* out of memory: return 0 */
        if (cache) {
            /* copy existing values */
            for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        /* zero the remaining slots */
        for (size_t i = cache_len; i < new_len; ++i) new_cache[i] = 0;
        cache = new_cache; cache_len = new_len;
    }

    /* Initialize a(0) if not already */
    if (cache_max == 0 && cache_len > 0 && cache[0] == 0)
        cache[0] = 1;

    /* If requested index already computed, return it */
    if (cache_max >= (size_t)n)
        return cache[(size_t)n];

    /* Compute terms from cache_max+1 up to n */
    for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
        /* sum of up to the previous 43 terms */
        seq_t sum = 0;
        size_t start = (i > 43) ? i - 43 : 0;
        for (size_t k = start; k < i; ++k)
            sum += cache[k];

        /* c(i) */
        seq_t c = 0;
        if (i == 0)            c = 1;
        else if (i <= 43)      c = 2;
        else if (i == 44)      c = 1;
        /* else c remains 0 for i > 44 */

        /* recurrence */
        seq_t val = c + 47 * sum;
        if (i >= 44)
            val -= 1128 * cache[i - 44];

        cache[i] = val;
    }

    cache_max = (size_t)n;
    return cache[(size_t)n];
}
