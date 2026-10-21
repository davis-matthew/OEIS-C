/*
 * A099253  (7*n+6)-th terms of expansion of 1/(1-x-x^7).
 * https://oeis.org/A099253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) -21*a(n-2) +35*a(n-3) -35*a(n-4) +21*a(n-5)
 *            -7*a(n-6) +a(n-7) initial values: a(0)=1, a(1)=8, a(2)=43,
 *            a(3)=211, a(4)=1030, a(5)=5055, a(6)=24851
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 7;                               /* storage for first 7 terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                 /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)43;
        cache[3] = (seq_t)211;
        cache[4] = (seq_t)1030;
        cache[5] = (seq_t)5055;
        cache[6] = (seq_t)24851;
        cache_len = 7;
    }

    size_t target = (size_t)n;
    if (target < cache_len)
        return cache[target];

    if (target + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= target)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_len; i <= target; ++i) {
        seq_t val = (seq_t)0;
        /* positive contributions */
        val += (seq_t)8 * cache[i - 1];
        val += (seq_t)35 * cache[i - 3];
        val += (seq_t)21 * cache[i - 5];
        val += cache[i - 7];
        /* negative contributions */
        val -= (seq_t)21 * cache[i - 2];
        val -= (seq_t)35 * cache[i - 4];
        val -= (seq_t)7  * cache[i - 6];
        cache[i] = val;
    }
    cache_len = target + 1;
    return cache[target];
}
