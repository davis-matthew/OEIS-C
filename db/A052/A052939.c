/*
 * A052939  Expansion of (1-x)*(1+x)/(1-3*x-x^2+2*x^3).
 * https://oeis.org/A052939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-3), a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* Lazy initialization of the cache */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* Out of memory: unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
    }

    /* Extend the cache if necessary */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        size_t new_mem = (size_t)(n + 1) * sizeof(seq_t);
        seq_t *tmp = (seq_t*)realloc(cache, new_mem);
        if (!tmp) return (seq_t)0; /* Out of memory */
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t val = (seq_t)3 * cache[i - 1] + cache[i - 2] - (seq_t)2 * cache[i - 3];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
