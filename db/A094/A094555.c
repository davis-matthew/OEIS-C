/*
 * A094555  Number of walks of length n between two vertices on the same
 *          triangular face of a truncated tetrahedron (triangular prism).
 * https://oeis.org/A094555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) - 6*a(n-3) with initial values
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)6;

    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of stored terms */
    static idx_t cache_capacity = 0;  /* allocated capacity */

    /* Initial allocation and base values. */
    if (cache == NULL) {
        cache_capacity = 4;                   /* enough for indices 0..3 */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)6;
        cache_size = 4;                       /* next index to compute */
    }

    /* Extend cache if necessary. */
    if (n + 1 > cache_size) {
        /* Ensure capacity is large enough for indices up to n. */
        idx_t new_capacity = cache_capacity;
        while (new_capacity <= n)
            new_capacity = (new_capacity == 0) ? 4 : new_capacity * 2;

        if (new_capacity > cache_capacity) {
            cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
            cache_capacity = new_capacity;
        }

        /* Compute terms from the end of the cache up to n. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term =
                cache[i - 1] * (seq_t)2 +          /* 2*a(i-1) */
                cache[i - 2] * (seq_t)5;          /* 5*a(i-2) */

            term -= cache[i - 3] * (seq_t)6;      /* subtract 6*a(i-3) */
            cache[i] = term;
        }
        cache_size = n + 1;                      /* all indices up to n are ready */
    }

    return cache[n];
}
