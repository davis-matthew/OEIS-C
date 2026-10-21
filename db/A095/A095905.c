/*
 * A095905  Sequence generated from Golomb's proof of de Bruijn's theorem on a
 *          torus considered as a matrix.
 * https://oeis.org/A095905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) + 18*a(n-2), a(1) = 1, a(2) = 21
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*
 * Compute the n-th term of OEIS sequence A095905.
 * Works for any seq_t and idx_t types defined by the build.
 * The function uses a simple memoised recurrence, growing an internal cache
 * each time a new term is requested.
 */
seq_t a(idx_t n)
{
    /* static cache stores values from 1 .. cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of filled entries in cache */

    /* Basic base cases (handled even when cache is not yet initialised) */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)21;

    /* Initialise cache on first use */
    if (cache_size == 0) {
        /* allocate room for indices 1 and 2 (index 0 unused) */
        cache_size = 2;
        /* malloc returns void*, cast to seq_t* for C++ compatibility */
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache)          /* unlikely, but guard against failure */
            return (seq_t)0;
        /* set initial values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)21;
    }

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* Grow cache to accommodate up to index n */
    if (n > cache_size) {
        /* realloc handles freeing old memory; cast its result */
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)          /* allocation failed */
            return (seq_t)0;
        cache = new_cache;

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)15 * cache[i - 1] + (seq_t)18 * cache[i - 2];
        }
        cache_size = n;
    }

    return cache[n];
}
