/*
 * A024741  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=2, a(2)=1, and a(3)=3.
 * https://oeis.org/A024741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1); a(1)=2,
 *            a(2)=1, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;   /* 1‑based indexing, cache[0] unused */
    static idx_t capacity = 0;    /* current allocated size */
    static idx_t cached_max = 0;  /* largest n with cache[n] valid */

    if (n < 1) return 0;          /* out of range – not used by the tests */

    /* return the initial values directly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* allocate or grow the cache to fit n */
    if (cache == NULL) {
        /* first time */
        capacity = n + 1;
        if (capacity < 4) capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cached_max = 3;
    } else if (n > capacity - 1) {
        /* need more space */
        size_t new_capacity = n + 1;
        if (new_capacity < capacity * 2) new_capacity = capacity * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failed */
        cache = new_cache;
        capacity = new_capacity;
        /* re‑establish the base values (they were guaranteed to be set) */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
    }

    /* compute missing terms up to n */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t sum = 0;
        idx_t limit = (k + 1) / 4;          /* floor((k+1)/4) */
        for (idx_t i = 1; i <= limit; ++i) {
            idx_t left  = 2 * i - 1;         /* odd index */
            idx_t right = k - 2 * i + 1;     /* even index */
            sum += cache[left] * cache[right];
        }
        cache[k] = sum;
    }
    cached_max = n;
    return cache[n];
}
