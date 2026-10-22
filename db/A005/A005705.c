/*
 * A005705  Number of partitions of 4*n into powers of 4.
 * https://oeis.org/A005705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(floor(n/4)), a(0) = 1
 * Range:     u8 n <= 35, u16 n <= 242, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len) {
        return cache[n];
    }

    idx_t new_len = n + 1;

    if (cache == NULL) {
        cache = alloc_zero(new_len);
        cache[0] = 1;
        cache_len = 1;
    } else {
        cache = grow_zero(cache, cache_len, new_len);
    }

    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = cache[i - 1] + cache[i / 4];
    }

    cache_len = new_len;
    return cache[n];
}
