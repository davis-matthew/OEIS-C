/*
 * A004626  Divisible only by primes congruent to 3 mod 8.
 * https://oeis.org/A004626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers divisible only by primes congruent to 3 mod 8
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 1000 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool is_only_primes_3_mod_8(uint64_t m) {
    if (m == 1) return true;
    
    uint64_t temp = m;
    
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (p % 8 != 3) return false;
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }
    
    if (temp > 1 && temp % 8 != 3) return false;
    
    return true;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_capacity = 0;
    
    if (n <= cache_len) {
        return cache[n - 1];
    }
    
    if (cache == NULL) {
        cache_capacity = 1000;
        cache = mem_new(seq_t, cache_capacity);
        cache_len = 0;
    }
    
    uint64_t current = (cache_len > 0) ? (uint64_t)cache[cache_len - 1] : 0;
    
    while (cache_len < n) {
        current++;
        if (is_only_primes_3_mod_8(current)) {
            if (cache_len >= cache_capacity) {
                cache_capacity *= 2;
                cache = mem_resize(seq_t, cache, cache_capacity);
            }
            cache[cache_len] = (seq_t)current;
            cache_len++;
        }
    }
    
    return cache[n - 1];
}
