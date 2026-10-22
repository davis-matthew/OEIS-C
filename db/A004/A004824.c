/*
 * A004824  Denominator of 2^n*(3*n-3)!/( ((n-1)!)^3 * (2*n)! ).
 * https://oeis.org/A004824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of 2^n * (3*n - 3)! / (((n-1)!)^3 * (2*n)!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   exponent_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/exponent_in_factorial.h"

static bool *sieve_of_eratosthenes(uint64_t limit) {
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return NULL;
    
    for (uint64_t i = 0; i <= limit; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;
    
    for (uint64_t i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    return is_prime;
}

static seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t n_val = (uint64_t)n;
    uint64_t limit = 3 * n_val - 3;
    if (2 * n_val > limit) limit = 2 * n_val;
    
    bool *is_prime = sieve_of_eratosthenes(limit);
    if (!is_prime) return 1;
    
    seq_t result = 1;
    
    for (uint64_t p = 2; p <= limit; p++) {
        if (!is_prime[p]) continue;
        
        int64_t exp_num = 0;
        if (p == 2) exp_num = n_val;
        exp_num += exponent_in_factorial(3 * n_val - 3, p);
        
        int64_t exp_denom = 3 * exponent_in_factorial(n_val - 1, p);
        exp_denom += exponent_in_factorial(2 * n_val, p);
        
        int64_t exp_reduced = exp_denom - exp_num;
        
        if (exp_reduced > 0) {
            result *= pow_seq((seq_t)p, (uint64_t)exp_reduced);
        }
    }
    
    free(is_prime);
    return result;
}
