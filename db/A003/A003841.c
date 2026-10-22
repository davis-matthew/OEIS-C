/*
 * A003841  Order of universal Chevalley group D_2(q), q = prime power.
 * https://oeis.org/A003841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (q * (q^2 - 1))^2 where q = A000961(n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 19, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static bool is_prime_power(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) {
        uint64_t temp = n;
        while (temp % 2 == 0) temp /= 2;
        return temp == 1;
    }
    
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t temp = n;
            while (temp % p == 0) temp /= p;
            return temp == 1;
        }
    }
    return true;
}

seq_t a(idx_t n) {
        
    static uint64_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n > cache_len) {
        idx_t new_len = (n < 10000) ? 10000 : n + 1000;
        if (cache == NULL) {
            cache = (uint64_t *)malloc(new_len * sizeof(uint64_t));
            cache_len = 0;
        } else {
            cache = (uint64_t *)realloc(cache, new_len * sizeof(uint64_t));
        }
        
        idx_t count = cache_len;
        uint64_t candidate = (cache_len > 0) ? (cache[cache_len - 1] + 1) : 2;
        
        while (count < new_len) {
            if (is_prime_power(candidate)) {
                cache[count] = candidate;
                count++;
            }
            candidate++;
        }
        cache_len = new_len;
    }
    
    uint64_t q_u64 = cache[n - 1];
    seq_t q = (seq_t)q_u64;
    seq_t q_squared = q * q;
    seq_t factor = q * (q_squared - 1);
    seq_t result = factor * factor;
    return result;
}
