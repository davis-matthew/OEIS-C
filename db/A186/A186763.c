/*
 * A186763  Number of increasing odd cycles in all permutations of
 *          {1,2,...,n}.
 * https://oeis.org/A186763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n*a(n-1) + a(n-2) - (n-2)*a(n-3),
   a(0)=0, a(1)=1, a(2)=2. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of already computed terms   */
    static idx_t cached_max = 0;   /* largest index for which term is computed */
    static idx_t cache_capacity = 0; /* allocated size of cache                 */

    /* First call: initialize cache for the first three terms. */
    if (cache == NULL) {
        cache_capacity = 3;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cached_max = 2;
    }

    /* If the requested value is already computed, return it. */
    if (n <= cached_max)
        return cache[n];

    /* Resize the cache if necessary. */
    if ((idx_t)(n + 1) > cache_capacity) {
        idx_t new_capacity = n + 1;
        cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        cache_capacity = new_capacity;
    }

    /* Compute terms from cached_max+1 up to n using the recurrence. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t term =
            (seq_t)i * cache[i - 1]          /* i * a(i-1) */
            + cache[i - 2]                   /* + a(i-2)   */
            - (seq_t)(i - 2) * cache[i - 3]; /* - (i-2)*a(i-3) */
        cache[i] = term;
    }

    cached_max = n;
    return cache[n];
}
