/*
 * A094364  Expansion of (1-5*x)/(1-10*x-100*x^2).
 * https://oeis.org/A094364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) + 100*a(n-2), a(0) = 1, a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for previously computed terms */
static seq_t *cache = NULL;          /* array of cached values          */
static size_t cache_cap = 0;         /* allocated capacity              */
static idx_t last_index = (idx_t)-1; /* highest index that has been computed */

/* initialise the cache with the first two terms */
static void init_cache(void) {
    if (cache != NULL) return;
    size_t init_cap = 4;                  /* start with a small power of two */
    cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
    cache_cap = init_cap;
    cache[0] = (seq_t)1;                  /* a(0) = 1 */
    cache[1] = (seq_t)5;                  /* a(1) = 5 */
    last_index = 1;
}

/* grow the cache so that it can hold “needed” elements          */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap < needed) new_cap <<= 1;          /* double until enough */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* compute all terms from last_index+1 up to n (inclusive) */
static void extend_to(idx_t n) {
    if (n <= last_index) return;
    size_t needed = (size_t)n + 1;
    ensure_capacity(needed);
    for (idx_t i = last_index + 1; i <= n; ++i) {
        cache[i] = 10 * cache[i - 1] + 100 * cache[i - 2];
    }
    last_index = n;
}

/* public function */
seq_t a(idx_t n) {
    if (cache == NULL) init_cache();
    extend_to(n);
    return cache[n];
}
