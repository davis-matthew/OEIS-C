/*
 * A116556  a(n) = 2*a(n-1) + 2*a(n-2), a(0)=0, a(1)=4.
 * https://oeis.org/A116556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2), a(0) = 0, a(1) = 4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the n-th term of A116556 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache of already computed terms */
    static size_t cache_cap = 0;     /* number of cached entries */

    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;

    /* Initial cache allocation */
    if (!cache) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        for (idx_t i = 2; i <= n; ++i) {
            seq_t t1 = cache[i-1] + cache[i-1];  /* 2*a(i-1) */
            seq_t t2 = cache[i-2] + cache[i-2];  /* 2*a(i-2) */
            cache[i] = t1 + t2;
        }
        return cache[n];
    }

    /* Resize the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        /* Compute new terms */
        for (idx_t i = cache_cap; i <= n; ++i) {
            seq_t t1 = cache[i-1] + cache[i-1];
            seq_t t2 = cache[i-2] + cache[i-2];
            cache[i] = t1 + t2;
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
