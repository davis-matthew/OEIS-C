/*
 * A005523  a(n) = maximal number of rational points on an elliptic curve over
 *          GF(q), where q = A246655(n) is the n-th prime power > 1.
 * https://oeis.org/A005523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q + 1 + floor(2*sqrt(q)) if (p does not divide
 *            floor(2*sqrt(q))) OR (q is a perfect square) OR (q is prime);
 *            otherwise a(n) = q + floor(2*sqrt(q)), where q = A246655(n) is
 *            the n-th prime power > 1. Based on Waterhouse 1969 theorem for
 *            maximal points on elliptic curves over finite fields
 * Range:     u8 n <= 62, u16 n <= 6587, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a000037_isqrt.h"


#define CACHE_SIZE 30000

static seq_t prime_powers[CACHE_SIZE];
static idx_t cache_count = 0;

static seq_t prime_of_prime_power(seq_t n) {
    if (n < 2) return 0;
    
    uint64_t limit = a000037_isqrt((uint64_t)n);
    
    for (uint64_t p_val = 2; p_val <= limit; p_val++) {
        seq_t p = (seq_t)p_val;
        if (n % p == 0) {
            seq_t m = n;
            while (m % p == 0) m /= p;
            if (m == 1) return p;
            return 0;
        }
    }
    
    return n;
}

static void fill_cache_to(idx_t target) {
    if (cache_count >= target) return;
    if (target > CACHE_SIZE) target = CACHE_SIZE;
    
    if (cache_count == 0) {
        prime_powers[0] = 2;
        cache_count = 1;
    }
    
    seq_t q = prime_powers[cache_count - 1] + 1;
    
    while (cache_count < target) {
        if (prime_of_prime_power(q)) {
            prime_powers[cache_count] = q;
            cache_count++;
        }
        q++;
    }
}

seq_t a(idx_t n) {
    fill_cache_to(n);
    if (n > cache_count) return 0;
    
    seq_t q = prime_powers[n - 1];
    seq_t p = prime_of_prime_power(q);
    
    seq_t s = (seq_t)a000037_isqrt((uint64_t)q);
    seq_t t = (q > s * (s + 1)) ? (2 * s + 1) : (2 * s);
    
    bool p_not_div_t = (t % p != 0);
    bool q_is_square = (s * s == q);
    bool q_is_prime = (q == p);
    
    if (p_not_div_t || q_is_square || q_is_prime) {
        return q + 1 + t;
    } else {
        return q + t;
    }
}
