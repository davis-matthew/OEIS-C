/*
 * A053404  Expansion of 1/((1+3*x)*(1-4*x)).
 * https://oeis.org/A053404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((4^(n+1)) - (-3)^(n+1))/7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n)=a(n-1)+12*a(n-2) for n>1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialize cache with first two terms */
    if (!cache) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;            /* unlikely to happen in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* cache[i] = cache[i-1] + 12 * cache[i-2]; */
            /* cast 12 to seq_t to make the multiplication work in both C and C++ */
            cache[i] = cache[i-1] + ((seq_t)12) * cache[i-2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
