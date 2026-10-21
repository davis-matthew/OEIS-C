/*
 * A200936  Successive values x of solutions Mordell's elliptic curve x^3-y^2
 *          = d contained points {x,y} with quadratic extension sqrt(2) over
 *          rationals.
 * https://oeis.org/A200936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+40*a(n-2)-40*a(n-3)-206*a(n-4)+206*a(n-5)+
 *            40*a(n-6)-40*a(n-7)-a(n-8)+a(n-9) initial values: a(0)=22,
 *            a(1)=190, a(2)=2878, a(3)=3862, a(4)=111382, a(5)=117118,
 *            a(6)=3864190, a(7)=3897622, a(8)=131738902, a(9)=131933758
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise the first 10 terms */
    if (!cache) {
        cache_len = 10;
        cache = seq_alloc(cache_len); /* array of cache_len elements */
        cache[0] = 22;   cache[1] = 190;   cache[2] = 2878;   cache[3] = 3862;
        cache[4] = 111382; cache[5] = 117118; cache[6] = 3864190;
        cache[7] = 3897622; cache[8] = 131738902; cache[9] = 131933758;
    }

    /* if the value is already cached, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* record current length before resizing */
    size_t prev_len = cache_len;
    size_t needed = (size_t)n + 1;

    /* grow the cache if necessary */
    if (needed > cache_len) {
        seq_t *new_cache = seq_realloc(cache, cache_len, needed);
        cache = new_cache;
        cache_len = needed;
    }

    /* compute all new terms up to n */
    for (size_t i = prev_len; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1]
                    + (seq_t)40 * cache[i-2]
                    - (seq_t)40 * cache[i-3]
                    - (seq_t)206 * cache[i-4]
                    + (seq_t)206 * cache[i-5]
                    + (seq_t)40 * cache[i-6]
                    - (seq_t)40 * cache[i-7]
                    - cache[i-8]
                    + cache[i-9];
        cache[i] = val;
    }

    return cache[(size_t)n];
}
