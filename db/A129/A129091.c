/*
 * A129091  a(0)=1; a(n) = gcd(a(n-1), n) + lcm(a(n-1), n).
 * https://oeis.org/A129091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = gcd(a(n-1), n) + lcm(a(n-1), n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialisation for a(0) */
    if (cache_len == 0) {
        cache = seq_realloc(NULL, 0, 1);
        cache_cap = 1;
        cache_len = 1;
        cache[0] = (seq_t)1;
    }

    /* grow cache and compute missing terms up to n */
    if ((size_t)n >= cache_len) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) return (seq_t)0; /* allocation failed */
            cache   = tmp;
            cache_cap = new_cap;
        }

        seq_t prev = cache[cache_len - 1];
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t idx_val = (seq_t)i;
            seq_t g = gcd(prev, idx_val);
            seq_t l = seq_mul(prev, idx_val) / g;   /* lcm = a(n-1)*n/gcd */
            seq_t val = g + l;
            cache[i] = val;
            prev = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
