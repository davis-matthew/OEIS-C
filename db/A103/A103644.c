/*
 * A103644  Expansion of g.f. (3x+1)/((1-3*x)*(1+5*x+9*x^2)).
 * https://oeis.org/A103644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-3) + 6*a(n-2) - 2*a(n-1) with a(0)=1, a(1)=1,
 *            a(2)=4
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global static cache for memoisation */
static seq_t *cache = NULL;        /* Pointer to cached values */
static idx_t last_computed = 2;    /* Largest n that has been computed */
static idx_t cache_capacity = 0;   /* Current allocation size */

seq_t a(idx_t n)
{
    /* Handle trivial cases directly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 4;

    /* Initialise or extend the cache if needed */
    if (!cache) {
        /* First power‑on initialisation */
        size_t cap = (size_t)(n + 1);
        if (cap < 3U) cap = 3U;           /* ensure space for initial terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);              /* abort on allocation failure */
        cache_capacity = (idx_t)cap;
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 4;
    } else if (n >= cache_capacity) {
        /* Grow the cache to include the requested index */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_capacity = (idx_t)new_cap;
    }

    /* Compute any missing values up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t term = 27 * cache[i - 3];
            term += 6 * cache[i - 2];
            term -= 2 * cache[i - 1];
            cache[i] = term;
        }
        last_computed = n;
    }

    return cache[n];
}
