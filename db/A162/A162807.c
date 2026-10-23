/*
 * A162807  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 21 a(2) = 420 a(3) = 8190 a(n) = 19*a(n-1) +
 *            19*a(n-2) - 190*a(n-3) for n >= 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;   /* memoisation table */
static size_t cache_cap = 0;  /* allocated size */
static size_t cache_len = 0;  /* number of elements currently stored */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)420;
    if (n == 3) return (seq_t)8190;

    /* Initialise the cache on first use */
    if (cache == NULL) {
        cache_cap = 4;          /* space for a0..a3 */
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort(); /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)420;
        cache[3] = (seq_t)8190;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* Grow the cache if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Constants for the recurrence */
    const seq_t C19  = (seq_t)19;
    const seq_t C190 = (seq_t)190;

    /* Compute missing terms up to n */
    for (idx_t i = 4; i <= n; ++i) {
        cache[(size_t)i] =
              C19  * cache[(size_t)(i-1)]
            + C19  * cache[(size_t)(i-2)]
            - C190 * cache[(size_t)(i-3)];
    }
    cache_len = needed;

    return cache[(size_t)n];
}
