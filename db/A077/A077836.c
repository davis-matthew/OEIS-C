/*
 * A077836  Expansion of (1-x)/(1-3*x-3*x^2-3*x^3).
 * https://oeis.org/A077836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + 3*a(n-3), a(0)=1, a(1)=2, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to handle out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    if (cache == NULL) {               /* first call – initialise */
        cache_size = 3;
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
    }

    if (n < cache_size)              /* already known */
        return cache[n];

    /* grow cache to accommodate up to n */
    size_t new_len = (size_t)(n + 1);
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1] + (seq_t)3 * cache[i-2] + (seq_t)3 * cache[i-3];
    }
    cache_size = n + 1;
    return cache[n];
}
