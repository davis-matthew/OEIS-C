/*
 * A052685  Expansion of e.g.f. (1-x^2)/(1-x-2*x^2+x^4).
 * https://oeis.org/A052685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n*a(n-1) + 2*n*(n-1)*a(n-2) - n*(n-1)*(n-2)*(n-3)*a(n-4)
 * with initial values a(0)=1, a(1)=1, a(2)=4, a(3)=24
 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache to allow non‑monotonic access */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache for the first call */
    if (cache == NULL) {
        cache_size = 4;              /* need indices 0..3 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;        /* out of memory – unlikely in tests */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 24;
    }

    /* grow cache if needed */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0; /* out of memory – unlikely */
        cache = tmp;
        cache_size = n + 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t term1 = (seq_t)i * cache[i - 1];
        seq_t term2 = (seq_t)2 * (seq_t)i * (seq_t)(i - 1) * cache[i - 2];
        seq_t term3 = (seq_t)i
                    * (seq_t)(i - 1)
                    * (seq_t)(i - 2)
                    * (seq_t)(i - 3)
                    * cache[i - 4];
        cache[i] = term1 + term2 - term3;
    }

    return cache[n];
}
