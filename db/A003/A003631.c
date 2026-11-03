/*
 * A003631  Primes congruent to 2 or 3 modulo 5.
 * https://oeis.org/A003631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 976 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static seq_t *alloc_array(idx_t n) {
    seq_t *p = mem_new(seq_t, (n + 1));
    for (idx_t i = 0; i <= n; i++) {
        p[i] = 0;
    }
    return p;
}

static seq_t *grow_arr(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *r = mem_new(seq_t, newlen);
    idx_t i;
    for (i = 0; i < oldlen; i++) r[i] = old[i];
    if (old) mem_free(old);
    return r;
}

/* Search for primes p such that p ≡ 2 or 3 (mod 5) */

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n > cache_len) {
        idx_t new_len = n;
        if (cache == NULL) {
            cache = alloc_array(new_len);
        } else {
            cache = grow_arr(cache, cache_len, new_len);
        }
        
        uint64_t candidate;
        if (cache_len == 0) {
            candidate = 2;
        } else {
            candidate = (uint64_t)cache[cache_len - 1] + 1;
        }
        
        idx_t count = cache_len;
        
        while (count < new_len) {
            if (is_prime_u64(candidate)) {
                uint64_t mod = candidate % 5;
                if (mod == 2 || mod == 3) {
                    cache[count] = (seq_t)candidate;
                    count++;
                }
            }
            candidate++;
        }
        
        cache_len = new_len;
    }
    
    return cache[n - 1];
}
