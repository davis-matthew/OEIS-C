/*
 * A003628  Primes congruent to {5, 7} mod 8.
 * https://oeis.org/A003628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3292, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n-th prime congruent to 5 or 7 mod 8 */

#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cached = NULL;
static idx_t cached_len = 0;
static idx_t cached_capacity = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cached == NULL) {
        cached_capacity = 128;
        cached = (seq_t *)malloc(sizeof(seq_t) * cached_capacity);
        if (!cached) return 0;
    }
    
    while (cached_len < n) {
        seq_t candidate;
        
        if (cached_len == 0) {
            candidate = 5;
        } else {
            candidate = cached[cached_len - 1] + 2;
        }
        
        while (1) {
            if (is_prime((uint64_t)candidate)) {
                seq_t mod8 = candidate % 8;
                if (mod8 == 5 || mod8 == 7) {
                    if (cached_len >= cached_capacity) {
                        idx_t new_capacity = cached_capacity * 2;
                        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_capacity);
                        if (!new_cache) return 0;
                        for (idx_t i = 0; i < cached_len; i++) {
                            new_cache[i] = cached[i];
                        }
                        free(cached);
                        cached = new_cache;
                        cached_capacity = new_capacity;
                    }
                    
                    cached[cached_len++] = candidate;
                    break;
                }
            }
            
            candidate += 2;
        }
    }
    
    return cached[n - 1];
}
