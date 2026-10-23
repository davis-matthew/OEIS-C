/*
 * A143449  Expansion of 1/(x^k*(1-x-2*x^(k+1))) for k=6.
 * https://oeis.org/A143449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 1 for 0 ≤ n ≤ 7 a(n) = a(n-1) + 2*a(n-7) for n ≥ 8
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 72, u64 n <= 147,
 *            u128 n <= 298, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;  /* array of terms */
static size_t cache_len  = 0;     /* number of terms stored (highest index + 1) */
static size_t cache_cap  = 0;     /* allocated capacity */

/* Return the n‑th term of A143449 */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;                     /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;      /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i <= 7) {
            /* 2*i + 1  – compute with seq_t arithmetic */
            cache[i] = (seq_t)(((seq_t)i * 2u) + 1u);
        } else {
            seq_t tmp = cache[i - 7];
            tmp += tmp;                        /* tmp = 2 * a(i-7) */
            cache[i] = cache[i - 1] + tmp;     /* a(i) = a(i-1) + 2*a(i-7) */
        }
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
