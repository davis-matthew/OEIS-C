/*
 * A004214  Positive numbers that are not the sum of three nonzero squares.
 * https://oeis.org/A004214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: positive integers not the sum of three nonzero squares */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/a000037_isqrt.h"

static bool isNotSumOfThree(seq_t num) {
    uint64_t n = (uint64_t)num;
    
    for (uint64_t x = 1; x * x + 2 <= n; x++) {
        uint64_t x_sq = x * x;
        for (uint64_t y = 1; x_sq + y * y + 1 <= n; y++) {
            uint64_t y_sq = y * y;
            uint64_t remainder = n - x_sq - y_sq;
            uint64_t z = a000037_isqrt(remainder);
            if (z * z == remainder) {
                return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cached_count = 0;
    
    if (cache != NULL && n <= cached_count) {
        return cache[n - 1];
    }
    
    if (cache == NULL) {
        cache_size = 1000;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    }
    
    seq_t num = (cached_count == 0) ? 1 : cache[cached_count - 1] + 1;
    
    while (cached_count < n) {
        if (cached_count >= cache_size) {
            cache_size = cache_size * 2;
            cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        }
        
        if (isNotSumOfThree(num)) {
            cache[cached_count] = num;
            cached_count++;
        }
        num++;
    }
    
    return cache[n - 1];
}
