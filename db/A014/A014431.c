/*
 * A014431  a(1) = 1, a(2) = 2, a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+
 *          a(n-2)*a(2) for n >= 3.
 * https://oeis.org/A014431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + … + a(n-2)*a(2) with a(1) =
 *            1, a(2) = 2 (offset 1). The sequence is computed lazily and
 *            memoised so calls in arbitrary order are efficient
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = 1, cache[2] = 2 */
static size_t cache_size = 0; /* number of allocated entries (including index 0) */

seq_t a(idx_t n)
{
    if (n == 0)   /* a(0) is undefined; return 0 so the caller can ignore it */
        return (seq_t)0;

    /* if already computed, just return the cached value */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* grow the cache to hold indices up to n */
    size_t new_size = (size_t)n + 1;          /* indices 0 .. n retain */
    seq_t *new_cache = NULL;

    if (cache == NULL)                      /* first allocation */
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    else                                    /* reallocation */
        new_cache = (seq_t *)realloc(cache,
                                      new_size * sizeof(seq_t));

    if (new_cache == NULL)                  /* allocation failed */
        abort();

    /* initialise new entries */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i == 0) {
            /* unused entry; initialise to 0 to avoid indeterminate value */
            new_cache[i] = (seq_t)0;
        } else if (i == 1) {
            new_cache[i] = (seq_t)1;
        } else if (i == 2) {
            new_cache[i] = (seq_t)2;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= (idx_t)i - 2; ++j)
                sum += new_cache[j] * new_cache[(size_t)i - j];
            new_cache[i] = sum;
        }
    }

    cache = new_cache;
    cache_size = new_size;

    return cache[(size_t)n];
}
