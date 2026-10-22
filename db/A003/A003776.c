/*
 * A003776  Number of 2-factors in P_5 X P_2n.
 * https://oeis.org/A003776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) - 57*a(n-2) + 26*a(n-3), a(1) = 3, a(2) = 54,
 *            a(3) = 1140
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    if (n > cache_len) {
        cache = grow_zero(cache, cache_len, n);
        cache_len = n;
    }
    
    if (cache[n - 1] != 0) {
        return cache[n - 1];
    }
    
    seq_t result;
    if (n == 1) {
        result = 3;
    } else if (n == 2) {
        result = 54;
    } else if (n == 3) {
        result = 1140;
    } else {
        result = (seq_t)24 * a(n - 1) - (seq_t)57 * a(n - 2) + (seq_t)26 * a(n - 3);
    }
    
    cache[n - 1] = result;
    return result;
}
