/*
 * A087440  Expansion of (1-2*x-3*x^2)/((1-2*x)*(1-4*x)).
 * https://oeis.org/A087440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) (n>2) Initial values: a(0)=1, a(1)=4,
 *            a(2)=13
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t last_computed = 0;

/* Ensure the cache can hold at least indices up to n */
static void ensure_capacity(idx_t n) {
    if (n < cache_size) return;
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);          /* allocation failure */
    cache = tmp;
    cache_size = new_size;
}

/* Initialize cache with first three values */
static void init_cache(void) {
    ensure_capacity(2);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)13;
    last_computed = 2;
}

seq_t a(idx_t n) {
    if (cache == NULL) init_cache();

    /* Return precomputed value if available */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Ensure capacity */
    ensure_capacity(n);

    /* Compute missing terms using recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)(6 * cache[i - 1] - 8 * cache[i - 2]);
    }

    last_computed = n;
    return cache[n];
}
