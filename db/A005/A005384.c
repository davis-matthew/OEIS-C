/*
 * A005384  Sophie Germain primes p: 2p+1 is also prime.
 * https://oeis.org/A005384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   nth Sophie Germain prime p such that both p and 2p+1 are prime
 * Range:     u8 n <= 12, u16 n <= 527, u32 n <= 9804, u64 n <= 9804,
 *            u128 n <= 9804, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t highest_computed = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    // Ensure cache is allocated
    if (cache == NULL) {
        cache_len = (n < 1000) ? 1000 : n * 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            cache[i] = 0;
        }
    } else if (n > cache_len) {
        idx_t new_len = cache_len * 2;
        while (new_len < n) new_len *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        for (idx_t i = cache_len; i < new_len; i++) {
            new_cache[i] = 0;
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    // If already computed, return
    if (n <= highest_computed) {
        return cache[n - 1];
    }
    
    // Start searching from where we left off
    seq_t p;
    idx_t count;
    
    if (highest_computed == 0) {
        p = 2;
        count = 0;
    } else {
        p = cache[highest_computed - 1];
        count = highest_computed;
        p = (p == 2) ? 3 : p + 2;
    }
    
    // Search for Sophie Germain primes
    while (count < n) {
        if (is_prime((uint64_t)p)) {
            seq_t q = 2 * p + 1;
            if (is_prime((uint64_t)q)) {
                count++;
                cache[count - 1] = p;
                highest_computed = count;
            }
        }
        p = (p == 2) ? 3 : p + 2;
    }
    
    return cache[n - 1];
}
