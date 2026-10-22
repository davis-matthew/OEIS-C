/*
 * A004626  Divisible only by primes congruent to 3 mod 8.
 * https://oeis.org/A004626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers divisible only by primes congruent to 3 mod 8
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_only_primes_3_mod_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_only_primes_3_mod_8.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_capacity = 0;
    
    if (n <= cache_len) {
        return cache[n - 1];
    }
    
    if (cache == NULL) {
        cache_capacity = 1000;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
        cache_len = 0;
    }
    
    uint64_t current = (cache_len > 0) ? (uint64_t)cache[cache_len - 1] : 0;
    
    while (cache_len < n) {
        current++;
        if (is_only_primes_3_mod_8(current)) {
            if (cache_len >= cache_capacity) {
                cache_capacity *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_capacity);
            }
            cache[cache_len] = (seq_t)current;
            cache_len++;
        }
    }
    
    return cache[n - 1];
}
