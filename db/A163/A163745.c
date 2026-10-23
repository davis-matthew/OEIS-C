/*
 * A163745  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 41*a(n-1)+41*a(n-2)+41*a(n-3)+41*a(n-4)-861*a(n-5) for n
 *            >= 6 Initial values: a(0)=1, a(1)=43, a(2)=1806, a(3)=75852,
 *            a(4)=3185784, a(5)=133802025 The function memoizes computed
 *            values so that calls in any order are fast and correct for all n
 *            >= 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding computed terms 0 .. cache_size-1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first six terms if not done yet */
    if (cache == NULL) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)43;
        cache[2] = (seq_t)1806;
        cache[3] = (seq_t)75852;
        cache[4] = (seq_t)3185784;
        cache[5] = (seq_t)133802025;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_size) return cache[n];

    /* extend the cache up to index n (inclusive) */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;          /* allocation failed */
    cache = new_cache;
    /* compute new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence only valid for i >= 6; for i==6..n we can safely apply it */
        seq_t term = ((seq_t)41) * cache[i-1];
        term = term + ((seq_t)41) * cache[i-2];
        term = term + ((seq_t)41) * cache[i-3];
        term = term + ((seq_t)41) * cache[i-4];
        term = term - ((seq_t)861) * cache[i-5];
        cache[i] = term;
    }
    cache_size = new_size;
    return cache[n];
}
