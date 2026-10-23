/*
 * A166474  a(1)=1; a(2)=2; for n>2, a(n) = a(n-1) + A000217(n-1)*a(n-2).
 * https://oeis.org/A166474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1,  a(2) = 2;
   for n > 2,  a(n) = a(n-1) + A000217(n-1) * a(n-2),
   where A000217(k) = k*(k+1)/2.  Indexing starts at 1. */
#include <stdlib.h>   /* malloc, free, realloc */

/* Cache the computed values of the sequence so that calls may
   be in arbitrary order.  The cache grows as needed. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;      /* pointer to cached terms */
    static idx_t cache_cap = 0;      /* currently allocated size */
    static idx_t computed_max = 0;   /* highest n whose value is known */

    /* undefined for n < 1 – behaviour below the offset does not matter */
    if (n == 0) return 0;

    /* initialise cache with the first two terms on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory: return 0 as fallback */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        computed_max = 2;
    }

    /* enlarge cache if needed */
    if (n > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        cache = new_cache;
        cache_cap = n;
    }

    /* if the requested value is already known, return it */
    if (n <= computed_max) return cache[n - 1];

    /* compute missing terms from computed_max+1 up to n */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        /* triangular number T(k-1) = (k-1)*k/2 */
        idx_t tri_ld = (k - 1) * k / 2;
        seq_t tri = (seq_t)tri_ld;

        /* recurrence: a(k) = a(k-1) + T(k-1) * a(k-2) */
        seq_t prod = tri * cache[k - 3];
        cache[k - 1] = cache[k - 2] + prod;
    }

    computed_max = n;
    return cache[n - 1];
}
