/*
 * A053565  a(n) = 2^(n-1)*(3*n-4).
 * https://oeis.org/A053565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^(n-1) * (3*n - 4) = 4*a(n-1) - 4*a(n-2), a(0) = -2,
 *            a(1) = -1
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 25, i64 n <= 56, i128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static cache growing dynamically.
   Each new term is computed from the recurrence, which only uses
   addition, subtraction, and multiplication by the small constant 4,
   guaranteeing correctness for any supported seq_t width. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache      = NULL;   /* values */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  cache_max  = 0;      /* highest index stored */

    /* Initialise the cache if this is the first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory – unlikely */
        cache[0] = (seq_t)-2;
        cache[1] = (seq_t)-1;
        cache_max = 1;
    }

    /* Base term for n=0 */
    if (n == 0) return (seq_t)-2;

    /* Base term for n=1 */
    if (n == 1) return (seq_t)-1;

    /* Ensure cache big enough to hold an entry for index n */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If already computed, just return */
    if (n <= cache_max) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* a(i) = 4*a(i-1) - 4*a(i-2) */
        cache[i] = ((seq_t)4) * cache[i-1] - ((seq_t)4) * cache[i-2];
    }
    cache_max = n;

    return cache[n];
}
