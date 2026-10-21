/*
 * A052592  E.g.f. (1-x)/(1-4x).
 * https://oeis.org/A052592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, for n>1 a(n)=3*4^(n-1)*n! Recurrence used:
 *            a(n+1) = (4+4*n)*a(n) (n>=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from OEIS A052592. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache      = NULL; /* array of stored terms */
    static size_t cache_size = 0;     /* number of cached terms */
    static size_t cache_cap  = 0;     /* allocated capacity */

    /* Return base case directly if n==0 */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Initialize cache if required */
    if (cache_size == 0) {
        cache_cap = 2;                    /* at least space for a(0) and a(1) */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) { /* out of memory – abort gracefully */
            abort();
        }
        cache[0] = (seq_t)1;             /* a(0) */
        cache[1] = (seq_t)3;             /* a(1) */
        cache_size = 2;
    }

    /* Grow capacity if the required index exceeds current capacity */
    if ((size_t)(n) + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n) + 1) {
            new_cap = (size_t)(n) + 1;
        }
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!cache) { /* out of memory – abort gracefully */
            abort();
        }
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        /* multiplier = 4 + 4*(i-1) */
        seq_t mult = (seq_t)(i - (idx_t)1) * (seq_t)4 + (seq_t)4;
        cache[i] = cache[i - (idx_t)1] * mult;
    }
    cache_size = (size_t)(n) + 1;

    return cache[(size_t)n];
}
