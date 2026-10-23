/*
 * A130667  a(1) = 1; a(n) = max{ 5*a(k) + a(n-k) | 1 <= k <= n/2 } for n > 1.
 * https://oeis.org/A130667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2*n) = 6*a(n); a(2*n+1) = 5*a(n) + a(n+1); (even and
 *            odd recursion)
 * Range:     u8 n <= 10, u16 n <= 93, u32 n <= 6845, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of already computed terms.  Index 0 is unused. */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;      /* allocated size (>= last_computed+1) */
static idx_t  last_computed = 0;   /* highest n whose value is valid */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t newsize = (cache_size > 0) ? cache_size : 2;
    while (newsize <= n)        /* grow by factor 2 until enough */
        newsize <<= 1;

    /* Use a C‑style cast so the code is valid for both C and C++ */
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp)
        abort();                /* allocation failed */
    cache = tmp;
    cache_size = newsize;
}

/* Return the n‑th term of sequence A130667.  Valid for all n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined, but safe return for n<=0 */

    /* Initialise cache on first call. */
    if (!cache) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));  /* indices 0..1 */
        if (!cache)
            abort();
        cache[1] = (seq_t)1;                 /* a(1) = 1 */
        cache_size = 2;
        last_computed = 1;
    }

    ensure_cache(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if ((i & 1) == 0) {                    /* even i : a(2*k) */
            cache[i] = (seq_t)6 * cache[i >> 1];
        } else {                               /* odd i : a(2*k+1) */
            idx_t k = i >> 1;                  /* floor(i/2) */
            cache[i] = (seq_t)5 * cache[k] + cache[k + 1];
        }
    }

    last_computed = n;
    return cache[n];
}
