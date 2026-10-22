/*
 * A004436  Numbers that are not the sum of 3 distinct squares.
 * https://oeis.org/A004436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) finds the n-th positive integer that cannot be expressed as
 *            the sum of 3 distinct squares
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static uint64_t isqrt_u64(uint64_t x) {
    if (x == 0) return 0;
    uint64_t root = 1ULL << ((63 - __builtin_clzll(x)) / 2 + 1);
    while (1) {
        uint64_t next = (root + x / root) / 2;
        if (next >= root) return root;
        root = next;
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_count = 0;
    
    if (cache == NULL || cache_count < n) {
        uint64_t sieve_limit = (uint64_t)(n * 1.8) + 1000;
        
        bool *reachable = (bool *)calloc((size_t)(sieve_limit + 1), sizeof(bool));
        if (reachable == NULL) {
            seq_t k = 0;
            idx_t count = 0;
            
            while (count < n) {
                k++;
                bool found = false;
                
                uint64_t sqrt_k = isqrt_u64((uint64_t)k);
                for (uint64_t i = 0; i <= sqrt_k && !found; i++) {
                    uint64_t i_sq = i * i;
                    if (i_sq > (uint64_t)k) break;
                    
                    for (uint64_t j = i + 1; j * j <= (uint64_t)k - i_sq && !found; j++) {
                        uint64_t j_sq = j * j;
                        uint64_t remainder = (uint64_t)k - i_sq - j_sq;
                        uint64_t l = isqrt_u64(remainder);
                        
                        if (l > j && l * l == remainder) {
                            found = true;
                        }
                    }
                }
                
                if (!found) count++;
            }
            return k;
        }
        
        for (uint64_t i = 0; i * i <= sieve_limit; i++) {
            uint64_t i_sq = i * i;
            for (uint64_t j = i + 1; i_sq + j * j <= sieve_limit; j++) {
                uint64_t j_sq = j * j;
                for (uint64_t l = j + 1; i_sq + j_sq + l * l <= sieve_limit; l++) {
                    uint64_t sum = i_sq + j_sq + l * l;
                    if (sum <= sieve_limit) {
                        reachable[sum] = true;
                    }
                }
            }
        }
        
        if (cache == NULL) {
            cache = (seq_t *)malloc((size_t)(n + 100) * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (size_t)(n + 100) * sizeof(seq_t));
        }
        
        if (cache == NULL) {
            free(reachable);
            return 0;
        }
        
        idx_t count = 0;
        for (uint64_t k = 1; count < n && k <= sieve_limit; k++) {
            if (!reachable[k]) {
                cache[count] = (seq_t)k;
                count++;
            }
        }
        
        cache_count = count;
        free(reachable);
    }
    
    if (n > cache_count) {
        seq_t k = cache[cache_count - 1];
        idx_t count = cache_count;
        
        while (count < n) {
            k++;
            bool found = false;
            
            uint64_t sqrt_k = isqrt_u64((uint64_t)k);
            for (uint64_t i = 0; i <= sqrt_k && !found; i++) {
                uint64_t i_sq = i * i;
                if (i_sq > (uint64_t)k) break;
                
                for (uint64_t j = i + 1; j * j <= (uint64_t)k - i_sq && !found; j++) {
                    uint64_t j_sq = j * j;
                    uint64_t remainder = (uint64_t)k - i_sq - j_sq;
                    uint64_t l = isqrt_u64(remainder);
                    
                    if (l > j && l * l == remainder) {
                        found = true;
                    }
                }
            }
            
            if (!found) count++;
        }
        return k;
    }
    
    return cache[n - 1];
}
