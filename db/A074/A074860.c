/*
 * A074860  a(n) = a(n-1) + R(a(n-2)) + R(a(n-3)) where a(0)=a(1)=a(2)=1 and
 *          R(k) = A004086(k) is the reverse of k.
 * https://oeis.org/A074860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = a(n-1) + R(a(n-2)) + R(a(n-3)), a(0) = a(1) = a(2) = 1;
 *            R(k) = reverse decimal digits of k
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of terms that have been computed */

    /* initialise first terms on first call */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 1;                 /* graceful failure */
        cache[0] = cache[1] = cache[2] = 1;
    }

    /* compute any missing terms up to n */
    if ((idx_t)cache_len <= n) {
        /* grow the array if necessary */
        size_t cap = cache_len;
        if (cap == 0) cap = 1;
        while (cap <= (size_t)n) cap <<= 1;
        if (cap > cache_len) {
            seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) return 1;               /* graceful failure */
            cache = tmp;
        }

        /* compute terms from cache_len to n inclusive */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t v = cache[i-1];
            v += reverse(cache[i-2]);          /* R(a(i-2)) */
            v += reverse(cache[i-3]);          /* R(a(i-3)) */
            cache[i] = v;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
