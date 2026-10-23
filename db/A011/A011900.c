/*
 * A011900  a(n) = 6*a(n-1) - a(n-2) - 2 with a(0) = 1, a(1) = 3.
 * https://oeis.org/A011900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 2, a(0)=1, a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t   cache_cap = 0;
static size_t   cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    if ((size_t)n >= cache_len) {
        size_t start = cache_len;
        if ((size_t)n >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t t = cache[i-1];
            t *= (seq_t)6;
            t -= cache[i-2];
            t -= (seq_t)2;
            cache[i] = t;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
