/*
 * A057090  Scaled Chebyshev U-polynomials evaluated at i*sqrt(7)/2.
 *          Generalized Fibonacci sequence.
 * https://oeis.org/A057090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057090: a(n) = 7 * (a(n-1) + a(n-2)),   a(0)=1, a(1)=7.
   Implements memoised recurrence with dynamic array */
#include <stdlib.h>

/* Shared static storage for memoisation */
static seq_t *cache = NULL;   /* array of computed terms          */
static idx_t  cache_len = 0;  /* number of terms currently stored */
static idx_t  capacity   = 0;  /* allocated size (in elements)    */

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (cache == NULL) {
        capacity   = 2;                               /* at least a0,a1 */
        cache      = (seq_t*)malloc(capacity * sizeof(seq_t));
        cache[0]   = (seq_t)1;
        cache[1]   = (seq_t)7;
        cache_len  = 2;                               /* two values computed */
    }

    /* If requested index exceeds current capacity grow it */
    if (n >= capacity) {
        idx_t newcap = capacity;
        while (newcap <= n) newcap <<= 1;            /* double until large enough */
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (cache == NULL) abort();                  /* allocation failure   */
        capacity = newcap;
    }

    /* Return cached value if already known */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to index n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)7 * (cache[i-1] + cache[i-2]);
    }

    cache_len = n + 1;   /* now indices 0..n are stored */

    return cache[(size_t)n];
}
