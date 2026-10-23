/*
 * A175214  a(1)=1; for n > 1, a(n) = a(floor(sqrt(n))) + a(floor(sqrt(n-1))).
 * https://oeis.org/A175214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = a(floor(sqrt(n))) +
 *            a(floor(sqrt(n-1)))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;       /* 1‑based cache of computed a(n) values */
static idx_t  cache_capacity = 0; /* allocated size (>= needed index) */
static idx_t  cache_size     = 0; /* largest index that has been computed */

/* Integer square root for unsigned 64‑bit values, using Newton’s method */
static idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Ensure that the cache can hold indices up to n; extend if necessary */
static void ensure_capacity(idx_t n) {
    if (n <= cache_capacity) return;
    idx_t new_cap = (cache_capacity < 8) ? 8 : cache_capacity;
    while (new_cap < n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) abort();         /* out of memory */
    cache = new_cache;
    cache_capacity = new_cap;
    for (idx_t i = cache_size + 1; i <= new_cap; ++i) cache[i] = 0;
}

/* Compute a(k) for all k up to n, filling the cache */
static void compute_up_to(idx_t n) {
    if (n <= cache_size) return;
    ensure_capacity(n);
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
        } else {
            idx_t s1 = isqrt_u64(i);
            idx_t s2 = isqrt_u64(i - 1);
            cache[i] = cache[s1] + cache[s2];
        }
    }
    cache_size = n;
}

/* Public entry point required by the problem statement */
seq_t a(idx_t n) {
    if (n < 1) return 0;      /* undefined for 0, but safe fallback */
    compute_up_to(n);
    return cache[n];
}
