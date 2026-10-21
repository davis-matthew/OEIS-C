/*
 * A219700  Number of n X 4 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and columns, 0..1 n X 4 array.
 * https://oeis.org/A219700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) * - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n >
 *            13 * with initial values a(1)=4, a(2)=7, a(3)=25, a(4)=64,
 *            a(5)=158, * a(6)=374, a(7)=841, a(8)=1789, a(9)=3619,
 *            a(10)=7008, * a(11)=13059, a(12)=23510, a(13)=41018
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 56, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the nth term (1‑based) of OEIS A219700.  */
seq_t a(idx_t n)
{
    /* cache for already computed values, protected against re‑entrancy */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* fixed first 13 values */
    static const seq_t init[13] = {
        (seq_t)4,     (seq_t)7,     (seq_t)25,    (seq_t)64,
        (seq_t)158,   (seq_t)374,   (seq_t)841,   (seq_t)1789,
        (seq_t)3619,  (seq_t)7008,  (seq_t)13059, (seq_t)23510,
        (seq_t)41018
    };

    /* initialise the cache on the first call */
    if (cache_len == 0) {
        cache_cap = 13;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        /* the cast is required for a C++ build where seq_t may be a class */
        if (!cache) return (seq_t)0;   /* fallback on allocation failure */
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_len = 13;
    }

    /* if the requested term is not yet computed, extend the cache */
    if (n > cache_len) {
        /* increase storage capacity if necessary */
        if (n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
            if (!tmp) return (seq_t)0;   /* fallback on allocation failure */
            cache   = tmp;
            cache_cap = new_cap;
        }

        /* compute the missing terms using the recurrence */
        for (idx_t m = cache_len + 1; m <= n; ++m) {
            seq_t val = 0;
            val += (seq_t)9   * cache[m - 2];
            val -= (seq_t)36  * cache[m - 3];
            val += (seq_t)84  * cache[m - 4];
            val -= (seq_t)126 * cache[m - 5];
            val += (seq_t)126 * cache[m - 6];
            val -= (seq_t)84  * cache[m - 7];
            val += (seq_t)36  * cache[m - 8];
            val -= (seq_t)9   * cache[m - 9];
            val +=            cache[m -10];
            cache[m - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
