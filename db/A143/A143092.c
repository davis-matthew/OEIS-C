/*
 * A143092  a(n) = a(n - a(n - 1)) + a(a(floor(2*n/3))) with a(0)=a(1)=1.
 * https://oeis.org/A143092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n - a(n - 1)) + a(a(floor(2*n/3))) with a(0) = a(1) = 1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache      = NULL;
    static idx_t cache_cap   = 0;   /* allocated number of slots         */
    static idx_t max_computed = 0;  /* highest index that has a value    */

    /* initialise cache for the two base terms */
    if (cache_cap == 0) {
        size_t init_cap = 2; /* indices 0 and 1 */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_cap   = init_cap;
        max_computed = 1;
    }

    /* grow the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* compute any values that are still unknown */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* term1 = a(i - a(i-1)) */
            idx_t idx1 = i - (idx_t)cache[i - 1];
            seq_t term1 = cache[idx1];

            /* term2 = a(a(floor(2*i/3))) */
            idx_t idx2_base = (i * 2) / 3;          /* floor(2*i/3)            */
            seq_t val2 = cache[idx2_base];
            idx_t idx2 = (idx_t)val2;
            seq_t term2 = cache[idx2];

            cache[i] = term1 + term2;
        }
        max_computed = n;
    }

    return cache[n];
}
