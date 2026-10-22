/*
 * A004249  a(n) = (2^2^...^2) (with n 2's) + 1.
 * https://oeis.org/A004249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, a(n) = 2^(a(n-1)-1) + 1 for n >= 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   a000740_pow2, alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000740_pow2.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    if (n == 0) return 2;
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache = alloc_zero(10);
        cache_len = 10;
        cache[0] = 2;
    }
    
    if (n >= cache_len) {
        seq_t *new_cache = grow_zero(cache, cache_len, n + 1);
        cache = new_cache;
        cache_len = n + 1;
    }
    
    if (cache[n] != 0) {
        return cache[n];
    }
    
    seq_t prev = a(n - 1);
    seq_t exp = prev - 1;
    seq_t result = a000740_pow2((idx_t)exp) + 1;
    cache[n] = result;
    return result;
}
