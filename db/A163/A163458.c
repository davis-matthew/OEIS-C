/*
 * A163458  a(n) = 12*a(n-1) - 34*a(n-2) for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A163458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 34*a(n-2) for n > 1; a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow O(1) retrieval after the first
       computation of consecutive terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;  /* largest n that has been computed */

    /* initialisation on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();          /* abort on allocation failure */
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)7;          /* a(1) */
        cache_max = 1;
    }

    /* return if already cached */
    if (n <= cache_max)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();           /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cache_max+1 up to n */
    const seq_t C12 = (seq_t)12;
    const seq_t C34 = (seq_t)34;
    idx_t i;
    for (i = cache_max + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = C12 * prev1 - C34 * prev2;
    }
    cache_max = n;
    return cache[n];
}
