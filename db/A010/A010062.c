/*
 * A010062  a(0)=1; thereafter a(n+1) = a(n) + number of 1's in binary
 *          representation of a(n).
 * https://oeis.org/A010062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0)=1; a(n+1)=a(n)+popcount(a(n))
 * Range:     u8 n <= 70, u16 n <= 8492, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the number of set bits in a value of type seq_t.
   Assumes that bitwise operators &, >> are defined for seq_t. */
static seq_t popcount_seq_t(seq_t x) {
    seq_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

/* Return the n-th term of OEIS A010062.
   Uses static caching to handle non‑monotonic calls efficiently. */
seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* how many entries allocated */
    static idx_t cached_max = (idx_t)-1;  /* largest n cached */

    /* Allocate cache if this is the first call */
    if (cache == NULL) {
        cache_len = 1; /* at least space for a(0) */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;  /* a(0) */
        cached_max = 0;
    }

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        /* allocate a new array, copy old values, keep old (leak is harmless) */
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute any values up to n that are still missing */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t pc = popcount_seq_t(prev);
        cache[i] = prev + pc;
    }
    cached_max = n;

    return cache[n];
}
