/*
 * A007461  Shifts left under AND-convolution with itself.
 * https://oeis.org/A007461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n+1) = Sum_{k=0..n} (a(k) AND a(n-k)), a(0) = 1
 * Range:     u8 n <= 50, u16 n <= 222, u32 n <= 1253, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_array, grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_array.h"
#include "../utils/grow_arr.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_cap = 16;
        cache = alloc_array(cache_cap);
        cache[0] = 1;
        cache_len = 1;
    }

    if (n < cache_len) {
        return cache[n];
    }

    if (n + 1 > cache_cap) {
        idx_t newcap = cache_cap;
        while (newcap < n + 1) newcap *= 2;
        cache = grow_arr(cache, cache_cap, newcap);
        cache_cap = newcap;
    }

    for (idx_t i = cache_len; i <= n; i++) {
        seq_t result = 0;
        for (idx_t k = 0; k < i; k++) {
            result += cache[k] & cache[i - 1 - k];
        }
        cache[i] = result;
        cache_len = i + 1;
    }

    return cache[n];
}
