/*
 * A135431  a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) with a(0)=0, a(1)=1,
 *          a(2)=2 and a(3)=3.
 * https://oeis.org/A135431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) Initial values: a(0) =
 *            0, a(1) = 1, a(2) = 2, a(3) = 3
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 41, u64 n <= 82, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoization cache */
static seq_t *cache = NULL;   /* array of computed terms */
static idx_t cache_len = 0;   /* next free index (size of cache) */

/* Initialize the cache with the first four terms */
static void init_cache(void) {
    if (cache != NULL) return;          /* already done once */
    cache = (seq_t*)malloc(4 * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);     /* allocation failed */
    cache[0] = 0; /* a(0) */
    cache[1] = 1; /* a(1) */
    cache[2] = 2; /* a(2) */
    cache[3] = 3; /* a(3) */
    cache_len = 4;                        /* we have indices 0..3 stored */
}

/* Grow the cache so that it contains a(need) */
static void grow_cache(idx_t need) {
    if (need < cache_len) return;         /* already available */

    /* Grow capacity to at least need+1 */
    size_t new_cap = cache_len ? cache_len : 4;
    while (new_cap <= (size_t)need) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    /* Compute and store missing terms */
    for (idx_t i = cache_len; i <= need; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - cache[i-4];
    }
    cache_len = need + 1;                 /* now indices 0..need are valid */
}

/* Public function implementing the sequence */
seq_t a(idx_t n) {
    init_cache();                         /* ensure base values exist */
    if (n < cache_len) return cache[n];   /* already computed */
    grow_cache(n);                        /* compute up to n if needed */
    return cache[n];
}
