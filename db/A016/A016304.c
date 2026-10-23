/*
 * A016304  Expansion of 1/((1-2*x)*(1-6*x)*(1-7*x)).
 * https://oeis.org/A016304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 15 a(2) = 157 a(n) = 15*a(n-1) - 68*a(n-2) +
 *            84*a(n-3) for n >= 3 (This recurrence follows from the
 *            generating function 1/((1-2x)(1-6x)(1-7x)).)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of elements currently valid */
    static idx_t cache_cap = 0;      /* allocated capacity */

    /* if already computed, return immediately */
    if (n < cache_len)
        return cache[n];

    /* grow capacity as needed */
    idx_t desired = n + 1;
    if (desired > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap < desired) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failure: return 0 as error value */
            return 0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise the cache if it is empty */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
        if (n == 0) return cache[0];
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = 15;
        cache_len = 2;
    }
    if (cache_len == 2 && n >= 2) {
        cache[2] = 157;
        cache_len = 3;
    }

    /* fill in missing entries up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] * 15 - cache[i-2] * 68 + cache[i-3] * 84;
    }
    cache_len = desired;

    return cache[n];
}
