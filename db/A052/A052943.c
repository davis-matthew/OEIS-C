/*
 * A052943  Expansion of (1-x^2)/(1-2*x^2-x^3+x^5).
 * https://oeis.org/A052943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) + a(n-3) - a(n-5) initial values: a(0) = 1, a(1)
 *            = 0, a(2) = 1, a(3) = 1, a(4) = 2
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 105,
 *            u128 n <= 207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* direct return for the first five terms */
    switch (n) {
    case 0: return (seq_t)1;
    case 1: return (seq_t)0;
    case 2: return (seq_t)1;
    case 3: return (seq_t)1;
    case 4: return (seq_t)2;
    default: /* compute below */ ;
    }

    /* static cache of already computed terms */
    static seq_t *cache      = NULL;   /* pointer to cached terms */
    static idx_t cache_size  = 0;     /* number of terms cached (>=5) */

    /* initialise the cache on first call */
    if (cache_size == 0) {
        cache_size = 5;               /* store indices 0..4 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;  /* memory allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
    }

    /* grow the cache if the requested index is not yet cached */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return (seq_t)0;    /* memory allocation failed */
        cache = tmp;

        /* compute terms from the current size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t v = cache[i-2];   /* a(n-2) */
            v += v;                 /* times 2 */
            v += cache[i-3];        /* + a(n-3) */
            v -= cache[i-5];        /* - a(n-5) */
            cache[i] = v;
        }
        cache_size = new_size;
    }

    return cache[n];
}
