/*
 * A147838  a(n) = 8*a(n-1) - 6*a(n-2), a(0)=1, a(1)=6.
 * https://oeis.org/A147838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 6*a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 903 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)6;
    }

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t last_computed = 1;

    if (cache == NULL) {
        cache_size = 2;
        cache = mem_new(seq_t, cache_size);
        if (!cache) {
            exit(1);
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
    }

    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size * 2;
        if ((size_t)n >= new_size) {
            new_size = (size_t)n + 1;
        }
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) {
            exit(1);
        }
        cache = tmp;
        cache_size = new_size;
    }

    if (last_computed < n) {
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            cache[k] = (seq_t)8 * cache[k - 1] - (seq_t)6 * cache[k - 2];
        }
        last_computed = n;
    }

    return cache[n];
}
