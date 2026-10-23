/*
 * A144575  E,g.f.: exp(1-sqrt(1-2*x-3*x^2)).
 * https://oeis.org/A144575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence: * a(0)=1, a(1)=1, a(2)=5, a(3)=25 * a(n) =
 *            (2*n-3)*a(n-1) + (3*n^2-12*n+10)*a(n-2) * + 3*(2*n-3)*a(n-3) +
 *            9*(n-1)*(n-3)*a(n-4) for n ≥ 4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* array of computed terms */
static idx_t  cache_allocated = 0;  /* allocated size of cache */
static idx_t  cache_filled   = 0;   /* number of terms already computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    /* Allocate or grow the cache so that cache_allocated > n */
    if (cache_allocated > n)
        return;
    idx_t new_size = cache_allocated == 0 ? 4 : cache_allocated * 2;
    while (new_size <= n)
        new_size *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = new_cache;
    cache_allocated = new_size;

    /* Initialise the first four values if this is the first allocation */
    if (cache_filled == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)25;
        cache_filled = 4;   /* we have computed up to index 3 */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the trivial cases directly (they are already in the cache) */
    if (n < 4) {
        if (cache == NULL)
            ensure_cache_capacity(3);
        return cache[n];
    }

    ensure_cache_capacity(n);

    /* Return already computed values immediately */
    if (n < cache_filled)
        return cache[n];

    /* Compute missing terms up to n inclusively */
    for (idx_t k = cache_filled; k <= n; ++k) {
        seq_t term1 = (seq_t)(2 * k - 3) * cache[k - 1];

        seq_t coeff2 = (seq_t)(3 * k * k - 12 * k + 10);
        seq_t term2 = coeff2 * cache[k - 2];

        seq_t coeff3 = (seq_t)3 * (seq_t)(2 * k - 3);
        seq_t term3 = coeff3 * cache[k - 3];

        seq_t coeff4 = (seq_t)9 * (seq_t)(k - 1) * (seq_t)(k - 3);
        seq_t term4 = coeff4 * cache[k - 4];

        cache[k] = term1 + term2 + term3 + term4;
    }

    cache_filled = n + 1;
    return cache[n];
}
