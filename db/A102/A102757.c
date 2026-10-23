/*
 * A102757  a(n) = Sum_{i=0..n} C(n,i)^2 * i! * 3^i.
 * https://oeis.org/A102757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=0..n} C(n,i)^2 * i! * 3^i Recurrence: a(0)=1,
 *            a(1)=4, a(n)=2*(3*n-1)*a(n-1)-9*(n-1)^2*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size */
    static idx_t last_computed = (idx_t)-1; /* largest n already computed */

    /* initialize cache for n = 0 and 1 */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        last_computed = 1;
    }

    /* if requested n is already computed, just return it */
    if (n <= (idx_t)(cache_size - 1))
        return cache[n];

    /* resize cache to fit up to n */
    if (n >= (idx_t)cache_size) {
        /* extend cache to size n+1 */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – fall back to recomputing on its own */
            /* Very unlikely in normal use. */
            new_cache = cache;
        }
        cache = new_cache;
        cache_size = (size_t)(n + 1);
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t prev1 = cache[k - 1];
        seq_t prev2 = cache[k - 2];

        /* factors: 2*(3*k-1) and 9*(k-1)^2 */
        seq_t three_k_minus1 = (seq_t)3 * (seq_t)k - (seq_t)1;
        seq_t k_minus_1 = (seq_t)k - (seq_t)1;
        seq_t k_minus_1_sq = k_minus_1 * k_minus_1;

        /* recurrence: a(k) = 2*(3*k-1)*prev1 - 9*(k-1)^2*prev2 */
        cache[k] = ((seq_t)2 * three_k_minus1 * prev1) -
                   ((seq_t)9 * k_minus_1_sq * prev2);
    }

    last_computed = n;
    return cache[n];
}
