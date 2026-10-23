/*
 * A042598  Numerators of continued fraction convergents to sqrt(828).
 * https://oeis.org/A042598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2302*a(n-6) - a(n-12) initial values: a(0)=28, a(1)=29,
 *            a(2)=115, a(3)=259, a(4)=892, a(5)=1151, a(6)=65348, a(7)=66499,
 *            a(8)=264845, a(9)=596189, a(10)=2053412, a(11)=2649601
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* initial values for n = 0..11 */
    static const seq_t init[12] = {
        28ULL, 29ULL, 115ULL, 259ULL, 892ULL, 1151ULL,
        65348ULL, 66499ULL, 264845ULL, 596189ULL, 2053412ULL, 2649601ULL
    };

    if (n < 12) return init[n];

    /* first call: allocate and fill initial values */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();            /* out of memory */
        memcpy(cache, init, sizeof(init));
    }

    /* extend cache if needed */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) {
            free(cache);
            abort();
        }
        cache = tmp;
        cache_size = new_size;

        /* compute new terms from old_size to n inclusive */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t prod = cache[i - 6] * (seq_t)2302;
            seq_t val  = prod - cache[i - 12];
            cache[i] = val;
        }
    }

    /* return cached value */
    return cache[n];
}
