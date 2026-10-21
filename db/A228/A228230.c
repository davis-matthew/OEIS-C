/*
 * A228230  Recurrence a(n) = (1/2)*n*(n+1)*a(n-1) + 1 with a(0) = 1.
 * https://oeis.org/A228230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*n*(n+1)*a(n-1) + 1, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized computation of sequence A228230. */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored (cache_size = max_index+1) */
    static idx_t cache_cap  = 0;   /* allocated capacity in terms */

    if (n < cache_size)                /* value already computed */
        return cache[n];

    /* Ensure cache has room for n+1 terms. */
    idx_t needed = n + 1;
    if (needed > cache_cap) {
        /* grow capacity (exponential to keep reallocs low) */
        idx_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* bail out if malloc fails */
            abort();
        }
        cache        = tmp;
        cache_cap    = new_cap;
    }

    /* initialize first term if cache empty */
    if (cache_size == 0) {
        cache[0]      = 1;   /* a(0) = 1 */
        cache_size    = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i < needed; ++i) {
        seq_t coeff = ((seq_t)i * (seq_t)(i + 1)) / 2; /* (i*(i+1))/2  */
        cache[i] = coeff * cache[i - 1] + 1;
    }
    cache_size = needed;

    return cache[n];
}
