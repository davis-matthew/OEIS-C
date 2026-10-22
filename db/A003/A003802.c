/*
 * A003802  Order of universal Chevalley group A_4(q), q = prime power.
 * https://oeis.org/A003802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^10 * (q^2 - 1) * (q^3 - 1) * (q^4 - 1) * (q^5 - 1)
 *            where q = A000961(n+1) (the (n+1)-th prime power, 1-indexed)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 4, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool is_prime_power(uint64_t num) {
    if (num == 1) return true;
    
    uint64_t p = 0;
    for (uint64_t i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            p = i;
            break;
        }
    }
    if (p == 0) return true;
    
    uint64_t temp = num;
    while (temp % p == 0) {
        temp /= p;
    }
    return temp == 1;
}

static uint64_t nth_prime_power(uint64_t n) {
    static uint64_t *cache = NULL;
    static uint64_t cache_len = 0;
    
    if (n <= cache_len) {
        return cache[n - 1];
    }
    
    if (cache == NULL) {
        cache = (uint64_t *)malloc(n * sizeof(uint64_t));
    } else {
        cache = (uint64_t *)realloc(cache, n * sizeof(uint64_t));
    }
    
    uint64_t candidate = (cache_len > 0) ? cache[cache_len - 1] + 1 : 1;
    uint64_t count = cache_len;
    
    while (count < n) {
        if (is_prime_power(candidate)) {
            cache[count] = candidate;
            count++;
        }
        candidate++;
    }
    cache_len = n;
    
    return cache[n - 1];
}

seq_t a(idx_t n) {
    uint64_t q_u64 = nth_prime_power((uint64_t)n + 1);
    seq_t q = (seq_t)q_u64;
    
    seq_t q2 = q * q;
    seq_t q3 = q2 * q;
    seq_t q4 = q3 * q;
    seq_t q5 = q4 * q;
    seq_t q10 = q5 * q5;
    
    seq_t result = q10 * (q2 - 1) * (q3 - 1) * (q4 - 1) * (q5 - 1);
    return result;
}
