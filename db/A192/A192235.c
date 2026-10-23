/*
 * A192235  Constant term of the reduction of the n-th 2nd-kind Chebyshev
 *          polynomial by x^2 -> x+1.
 * https://oeis.org/A192235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) with initial
 *            values: a(1)=0, a(2)=3, a(3)=8, a(4)=21
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i), 1‑based indexing, 0 unused */
static idx_t cache_size = 0;  /* highest index currently cached (0 if empty) */

/* Ensure the cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* Resize the cache to hold indices 0..n (0 unused) */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();   /* terminate if memory allocation fails */
    cache = tmp;

    /* Compute all missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)0;
        } else if (i == 2) {
            cache[2] = (seq_t)3;
        } else if (i == 3) {
            cache[3] = (seq_t)8;
        } else if (i == 4) {
            cache[4] = (seq_t)21;
        } else {
            /* a(i) = 2*a(i-1) + 2*a(i-2) + 2*a(i-3) - a(i-4) */
            cache[i] = cache[i-1];              /* a(i-1)   */
            cache[i] += cache[i-1];             /* + a(i-1) */
            cache[i] += cache[i-2];             /* + a(i-2) */
            cache[i] += cache[i-2];             /* + a(i-2) */
            cache[i] += cache[i-3];             /* + a(i-3) */
            cache[i] += cache[i-3];             /* + a(i-3) */
            cache[i] -= cache[i-4];             /* - a(i-4) */
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for offset‑1 calls */
    ensure_cache(n);
    return cache[n];
}
