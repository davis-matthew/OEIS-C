/*
 * A108206  a(n)= 3*(n-1)*a(n-1) +(n-1)*a(n-2), with a(0)=1, a(1)=1.
 * https://oeis.org/A108206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-1)*a(n-1) + (n-1)*a(n-2) with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below caches previously computed terms so that
   subsequent calls with any order of indices remain fast. */
#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed values */
static idx_t  cache_len = 0;    /* number of valid entries in cache */

/* Ensure the cache is large enough to hold values up to idx */
static void ensure_cache(idx_t idx) {
    if (cache_len > idx) return;
    idx_t new_len = (cache_len == 0) ? 16 : cache_len * 2;
    while (new_len <= idx) new_len *= 2;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache_len = new_len;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache(n);

    /* Initialize base cases if they have not been set yet */
    if (cache_len > 1 && cache[0] == 0 && cache[1] == 1) {
        /* already initialized */
    } else {
        cache[0] = 0;
        cache[1] = 1;
    }

    /* Compute from the point after the last computed index */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        /* Compute term1 = 3*(i-1)*a[i-1] */
        seq_t factor1 = (seq_t)(3 * (i - 1));
        seq_t term1   = factor1 * cache[i - 1];

        /* Compute term2 = (i-1)*a[i-2] */
        seq_t factor2 = (seq_t)(i - 1);
        seq_t term2   = factor2 * cache[i - 2];

        cache[i] = term1 + term2;
    }

    return cache[n];
}
