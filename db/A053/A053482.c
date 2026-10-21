/*
 * A053482  Binomial transform of A029767.
 * https://oeis.org/A053482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)*a(n-1) - (2*n+3)*(n-1)*a(n-2) +
 *            2*(n-1)*(n-2)*a(n-3) Initial values: a(0) = 1, a(1) = 4, a(2) =
 *            21
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache: values computed for indices 0 .. cache_size-1 */
    static seq_t *cache   = NULL;
    static size_t cache_cap   = 0;   /* allocated capacity */
    static size_t cache_size = 0;   /* number of valid entries */

    /* initialize cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        assert(cache != NULL);
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)4;   /* a(1) */
        cache[2] = (seq_t)21;  /* a(2) */
        cache_size = 3;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* grow cached array if necessary */
    size_t needed_cap = (size_t)(n + 1);
    if (needed_cap > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        assert(new_cache != NULL);
        cache = new_cache;
        cache_cap = needed_cap;
    }

    /* compute missing terms up to index n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term1 = (seq_t)(3 * i + 1) * cache[i - 1];
        seq_t coeff2 = (seq_t)(2 * i + 3) * (i - 1);
        seq_t term2 = coeff2 * cache[i - 2];
        seq_t coeff3 = (seq_t)2 * (i - 1) * (i - 2);
        seq_t term3 = coeff3 * cache[i - 3];
        cache[i] = term1 - term2 + term3;
    }
    cache_size = n + 1;

    return cache[n];
}
