/*
 * A187830  a(n)=2*a(n-1)+(n+3)*a(n-2)-(n+3)*a(n-3), a(0)=0, a(1)=0, a(2)=1.
 * https://oeis.org/A187830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=2*a(n-1)+(n+3)*a(n-2)-(n+3)*a(n-3), a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized computation of a(n) */
seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of computed terms */

    /* --- initialization for first three terms --- */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;  /* fallback on allocation failure */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
    }

    /* Return precomputed value if available */
    if (n < cache_size)
        return cache[n];

    /* Extend cache up to n (inclusive) */
    idx_t new_size = n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;  /* fallback on allocation failure */
        cache = tmp;

        /* Compute terms from old cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t inc = (seq_t)i + 3;
            seq_t term;
            if (i == 0) term = (seq_t)0;
            else if (i == 1) term = (seq_t)0;
            else if (i == 2) term = (seq_t)1;
            else
                term = (seq_t)2 * cache[i-1] + inc * cache[i-2] - inc * cache[i-3];
            cache[i] = term;
        }
        cache_size = new_size;
    }

    return cache[n];
}
