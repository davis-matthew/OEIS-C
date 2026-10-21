/*
 * A052961  Expansion of (1 - 3*x) / (1 - 5*x + 3*x^2).
 * https://oeis.org/A052961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) with a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache for previously computed values */
static seq_t *a_cache   = NULL; /* pointing to allocated seq_t array */
static idx_t   cache_len = 0;   /* number of values currently stored   */
static idx_t   cache_cap = 0;   /* current allocation capacity        */

/* Ensure the cache has space for at least indices 0 .. n inclusive */
static void ensure_cache_capacity(idx_t n) {
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise base cases 0 and 1 */
static void initialise_base(void) {
    if (cache_len > 0) return;   /* already initialised */
    ensure_cache_capacity(1);
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)2;
    cache_len  = 2;
}

/* Public function required by the build system */
seq_t a(idx_t n) {
    initialise_base();

    if (n < cache_len) return a_cache[n];

    ensure_cache_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 5*a(i-1) - 3*a(i-2) */
        a_cache[i] = 5*a_cache[i-1] - 3*a_cache[i-2];
    }
    cache_len = n + 1;
    return a_cache[n];
}
