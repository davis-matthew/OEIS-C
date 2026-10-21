/*
 * A234273  G.f.: (1+x+x^2+x^3)/(1-x^2-2*x^3-x^4+x^6).
 * https://oeis.org/A234273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + a(n-4) - a(n-6) for n ≥ 4 Initial
 *            values: a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 4
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms         */
    static idx_t cache_size = 0;         /* allocated size of 'cache'      */
    static idx_t last_computed = (idx_t)-1; /* highest index already computed */

    /* First call: allocate and initialise the cache */
    if (cache == NULL) {
        /* Allocate enough space for at least the initial terms */
        idx_t init_size = 4;
        if (n + 1 > init_size)
            init_size = n + 1;            /* grow immediately if the first request is large */

        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;              /* out of memory → return 0 (acceptable for this use case) */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;

        cache_size    = init_size;
        last_computed = 3;
    }

    /* Return immediately if the requested term is already known */
    if (n < 4)
        return cache[n];

    /* Grow the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;              /* out of memory → return 0 */

        cache      = tmp;
        cache_size = new_size;
    }

    /* Compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 2];
        val += ((seq_t)2) * cache[i - 3];
        val += cache[i - 4];
        if (i >= 6)
            val -= cache[i - 6];

        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
