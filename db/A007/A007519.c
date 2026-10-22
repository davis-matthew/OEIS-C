/*
 * A007519  Primes of form 8n+1, that is, primes congruent to 1 mod 8.
 * https://oeis.org/A007519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes of form 8n+1. Generate candidates of form 8k+1 and test
 *            for primality
 * Range:     u8 n <= 10, u16 n <= 1614, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n) {
    while (cache_len < n) {
        if (cache_len >= cache_cap) {
            idx_t new_cap = (cache_cap == 0) ? 1000 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache) {
                for (idx_t i = 0; i < cache_len; i++) {
                    new_cache[i] = cache[i];
                }
                free(cache);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        
        seq_t candidate;
        if (cache_len == 0) {
            candidate = 1;
        } else {
            candidate = cache[cache_len - 1] + 8;
        }
        
        while (!is_prime((uint64_t)candidate)) {
            candidate += 8;
        }
        
        cache[cache_len++] = candidate;
    }
    
    return cache[n - 1];
}
