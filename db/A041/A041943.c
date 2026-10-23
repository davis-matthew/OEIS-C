/*
 * A041943  Denominators of continued fraction convergents to sqrt(494).
 * https://oeis.org/A041943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 146070*a(n-10) - a(n-20) for n > 19 with initial values
 *            a(0) … a(19) given below
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial 20 terms of the sequence. */
static const seq_t init_vals[20] = {
    1ULL, 4ULL, 9ULL, 22ULL, 31ULL, 84ULL, 115ULL, 314ULL, 743ULL, 3286ULL,
    145327ULL, 584594ULL, 1314515ULL, 3213624ULL, 4528139ULL,
    12269902ULL, 16798041ULL, 45865984ULL, 108530009ULL, 479986020ULL
};

seq_t a(idx_t n)
{
    /* Return values that are part of the seed. */
    if (n < 20)
        return init_vals[(size_t)n];

    /* Static cache for computed terms. */
    static seq_t *cache     = NULL;
    static size_t cache_sz  = 0;   /* current capacity of cache */
    static idx_t  cache_max = (idx_t)-1;  /* highest n that has been computed */

    /* Allocate or grow the cache as needed.  The capacity must be at least
       n+1 so that index n is valid. */
    if (cache == NULL) {
        cache_sz = (size_t)n + 1;
        if (cache_sz < 20)
            cache_sz = 20;          /* at least room for the seeds */
        cache = (seq_t*)malloc(cache_sz * sizeof(seq_t));
        if (!cache)
            return 0;              /* out of memory – caller should not crash */

        /* Copy the initial 20 values into the cache. */
        for (size_t i = 0; i < 20; ++i)
            cache[i] = init_vals[i];
        cache_max = 19;
    } else if ((size_t)n + 1 > cache_sz) {
        cache_sz = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, cache_sz * sizeof(seq_t));
        if (!cache)
            return 0;              /* out of memory */
    }

    /* If the requested term is already known, return it. */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* Ensure that terms for indices n-10 and n-20 are already present. */
    /* After the initial 20 terms all required terms are present because we
       compute in increasing order below. */
    const seq_t coeff = (seq_t)146070;

    for (idx_t i = cache_max + 1; i <= n; ++i)
        cache[(size_t)i] = coeff * cache[(size_t)(i - 10)] - cache[(size_t)(i - 20)];

    cache_max = n;
    return cache[(size_t)n];
}
