/*
 * A003963  Fully multiplicative with a(p) = k if p is the k-th prime.
 * https://oeis.org/A003963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   If n = product prime(k)^e(k) then a(n) = product k^e(k)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>


static bool is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    if (p == 3) return true;
    if (p % 3 == 0) return false;
    
    for (uint64_t i = 5; i * i <= p; i += 6) {
        if (p % i == 0 || p % (i + 2) == 0) return false;
    }
    return true;
}

static uint64_t count_primes(uint64_t p) {
    if (p < 2) return 0;
    
    uint64_t count = 1;
    if (p == 2) return count;
    
    for (uint64_t i = 3; i <= p; i += 2) {
        if (is_prime(i)) count++;
    }
    return count;
}

static seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }
            
            uint64_t k = count_primes(p);
            result *= pow_seq((seq_t)k, exp);
        }
    }
    
    if (m > 1) {
        uint64_t k = count_primes(m);
        result *= (seq_t)k;
    }
    
    return result;
}
