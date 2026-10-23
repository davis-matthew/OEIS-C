/*
 * A186842  Number of (n+1) X 2 binary arrays with no 2 X 2 subblock diagonal
 *          sum less antidiagonal sum equal to any horizontal or vertical
 *          neighbor 2 X 2 subblock diagonal sum less antidiagonal sum.
 * https://oeis.org/A186842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 10*a(n-3) + 8*a(n-4) with a(1)=16,
 *            a(2)=50, a(3)=162, a(4)=512 (OEIS sequence A186842)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;     /* array indexed by n (1‑based) */
static idx_t  cache_last = 0;   /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* Return 0 for out‑of‑range index; the problem guarantees n ≥ 1. */
    if (n == 0) return 0;

    /* Initialise base terms on first use */
    if (cache == NULL) {
        /* Allocate indices 0..4 (index 0 unused) */
        cache = (seq_t*)malloc(5 * sizeof(seq_t));
        if (cache == NULL) return 0;      /* allocation failure short‑circuit */
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)50;
        cache[3] = (seq_t)162;
        cache[4] = (seq_t)512;
        cache_last = 4;                   /* highest computed index so far   */
    }

    /* If the requested term is already cached, just return it */
    if (n <= cache_last) return cache[n];

    /* Grow the cache to accommodate up to n */
    size_t new_size = (size_t)n + 1;          /* indices 0..n */
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) return 0;          /* allocation failure */
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    idx_t i;
    for (i = cache_last + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 3*a(i-2) + 10*a(i-3) + 8*a(i-4) */
        cache[i] =
            cache[i-1] +
            (seq_t)3 * cache[i-2] +
            (seq_t)10 * cache[i-3] +
            (seq_t)8  * cache[i-4];
    }
    cache_last = n;
    return cache[n];
}
