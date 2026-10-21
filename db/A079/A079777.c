/*
 * A079777  a(0) = 0, a(1) = 1; for n > 1, a(n) = (a(n-1) + a(n-2)) (mod n).
 * https://oeis.org/A079777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; for n > 1, a(n) = (a(n-1) + a(n-2)) (mod n)
 * Range:     u8 n <= 80, u16 n <= 9731, u32 n <= 9731, u64 n <= 9731,
 *            u128 n <= 9731, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A079777 efficiently with memoization to avoid recomputation
   on out-of-order calls. Uses only +, -, *, /, % and simple datatype
   conversions, supporting all seq_t sizes (including arbitrary precision). */
#include <stdlib.h>      /* for malloc, realloc */

seq_t a(idx_t n) {
    if (n == 0u) return 0;
    if (n == 1u) return 1;

    /* Static cache: cache[i] holds a(i) for i < cache_len. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache for indices 0 and 1. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
    }

    /* If requested n is already computed, return it. */
    if (n < cache_len) return cache[n];

    /* Extend cache up to n. */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t ai = cache[i-1] % (seq_t)i;   /* a(i-1) modulo i */
        seq_t bi = cache[i-2] % (seq_t)i;   /* a(i-2) modulo i */
        seq_t sum = ai + bi;                /* sum < 2*i  (i ≤ 10000) */
        if (sum >= (seq_t)i) sum -= (seq_t)i; /* sum modulo i */
        cache[i] = sum;
    }
    cache_len = new_len;
    return cache[n];
}
