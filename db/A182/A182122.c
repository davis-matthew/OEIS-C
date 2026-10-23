/*
 * A182122  Expansion of exp( arcsinh( 2*x ) ).
 * https://oeis.org/A182122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) satisfies 2*a(n)+∑_{i=1}^{n-1} a(i)*a(n-i)=4*a(n-1), with
 *            a(0)=1. This gives a(n) = (4*a(n-1) - ∑_{i=1}^{n-1} a(i)*a(n-i))
 *            / 2 (n ≥ 1)
 * Range:     i8 n <= 11, i16 n <= 21, i32 n <= 39, i64 n <= 73, i128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers for memory allocation and string handling. */
#include <stdlib.h>
#include <string.h>

/*  Mutable cache for memoised coefficients. */
static seq_t *cache = NULL;        /* array of computed values */
static size_t cache_size = 0;      /* current allocation size */
static idx_t last_computed = (idx_t)-1;  /* index of the largest computed value */

/*  Ensure that the cache can hold indices up to n (inclusive).  */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)(n + 1));
        if (!cache) abort();
        cache_size = (size_t)(n + 1);
        return;
    }
    if (cache_size >= (size_t)(n + 1))
        return;
    /* Reallocate with the larger size. */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)(n + 1));
    if (!tmp) abort();
    cache = tmp;
    cache_size = (size_t)(n + 1);
}

/*  Main sequence function.  */
seq_t a(idx_t n) {
    /* Initialise the cache and base coefficient if needed. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        cache_size = 1;
        last_computed = 0;
    }

    /* Make the cache large enough. */
    ensure_cache(n);

    /* Compute missing values up to n. */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Sum_{k=1}^{i-1} a(k)*a(i-k) */
            seq_t s = 0;
            for (idx_t k = 1; k < i; ++k) {
                s += cache[k] * cache[i - k];
            }
            /* a(i) = (4*a(i-1) - s) / 2 */
            seq_t tmp = (seq_t)4 * cache[i - 1];
            cache[i] = (tmp - s) / (seq_t)2;
        }
        last_computed = n;
    }

    return cache[n];
}
