/*
 * A166999  a(n) = a(n-1) + a(n-1)^2 + a(n-1)^3 for n>0 with a(0)=1.
 * https://oeis.org/A166999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=a(n-1)+a(n-1)^2+a(n-1)^3 for n>0 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache == NULL) {
        cache_cap = 1;
        cache_len = 1;
        cache = (seq_t*) malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;          /* out‑of‑memory fallback */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
    }

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow cache if necessary */
    size_t desired = (size_t)n + 1;
    if (cache_cap < desired) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < desired)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*) realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i < desired; ++i) {
        seq_t last = cache[i-1];
        seq_t last2 = last * last;
        seq_t last3 = last2 * last;
        cache[i] = last + last2 + last3;
    }
    cache_len = desired;
    return cache[(size_t)n];
}
