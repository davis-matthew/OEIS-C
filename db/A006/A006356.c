/*
 * A006356  a(n) = 2*a(n-1) + a(n-2) - a(n-3) for n >= 3, starting with a(0) =
 *          1, a(1) = 3, and a(2) = 6.
 * https://oeis.org/A006356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, walk
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) for n >= 3, starting with a(0)
 *            = 1, a(1) = 3, a(2) = 6
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_arr.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache = grow_arr(NULL, 0, 3);
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 6;
        cache_len = 3;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    idx_t old_len = cache_len;
    cache = grow_arr(cache, cache_len, n + 1);
    cache_len = n + 1;
    
    for (idx_t i = old_len; i <= n; i++) {
        cache[i] = 2 * cache[i-1] + cache[i-2] - cache[i-3];
    }
    
    return cache[n];
}
