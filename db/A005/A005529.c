/*
 * A005529  Primitive prime factors of the sequence k^2 + 1 (A002522) in the
 *          order that they are found.
 * https://oeis.org/A005529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for each k >= 1, compute k^2 + 1, find its primitive prime
 *            factors (those not dividing m^2 + 1 for any m < k), and list
 *            them in order
 * Range:     u8 n <= 11, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root using Newton's method */
static seq_t isqrt_seq(seq_t n) {
    if (n <= 1) return n;
    
    seq_t x = n;
    seq_t y = (x + 1) / 2;
    
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    
    return x;
}

/* Get unique prime factors of n */
static void factorize(seq_t n, seq_t *factors, idx_t *count) {
    *count = 0;
    
    /* Check p = 2 */
    if (n % 2 == 0) {
        factors[(*count)++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    
    /* Check odd p up to sqrt(n) */
    seq_t limit = isqrt_seq(n);
    for (seq_t p = 3; p <= limit; p += 2) {
        if (n % p == 0) {
            factors[(*count)++] = p;
            while (n % p == 0) n /= p;
            limit = isqrt_seq(n);
        }
    }
    
    if (n > 1) {
        factors[(*count)++] = n;
    }
}

seq_t a(idx_t n) {
    static seq_t *primitive_factors = NULL;
    static seq_t *all_primes = NULL;
    static idx_t num_factors = 0;
    static idx_t num_all_primes = 0;
    static idx_t max_k = 0;
    static idx_t capacity = 0;
    
    if (primitive_factors == NULL) {
        capacity = 1024;
        primitive_factors = (seq_t *)malloc(capacity * sizeof(seq_t));
        all_primes = (seq_t *)malloc(capacity * sizeof(seq_t));
        num_factors = 0;
        num_all_primes = 0;
        max_k = 0;
    }
    
    /* Compute primitive factors until we have at least n */
    while (num_factors < n) {
        max_k++;
        seq_t k_sq_plus_1 = (seq_t)max_k * (seq_t)max_k + 1;
        
        seq_t factors[100];
        idx_t factor_count;
        factorize(k_sq_plus_1, factors, &factor_count);
        
        for (idx_t i = 0; i < factor_count; i++) {
            seq_t p = factors[i];
            
            /* Check if p has been seen before */
            bool seen = false;
            for (idx_t j = 0; j < num_all_primes; j++) {
                if (all_primes[j] == p) {
                    seen = true;
                    break;
                }
            }
            
            if (!seen) {
                /* p is primitive */
                if (num_factors >= capacity) {
                    capacity *= 2;
                    primitive_factors = (seq_t *)realloc(primitive_factors, capacity * sizeof(seq_t));
                    all_primes = (seq_t *)realloc(all_primes, capacity * sizeof(seq_t));
                }
                
                primitive_factors[num_factors++] = p;
                all_primes[num_all_primes++] = p;
            }
        }
    }
    
    return primitive_factors[n - 1];
}
