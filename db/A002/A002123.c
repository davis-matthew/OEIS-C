/*
 * A002123  a(1) = 0, a(2) = 0; for n > 2, a(n) - a(n-3) - a(n-5) - ... -
 *          a(n-p) = n if n is prime, otherwise = 0, where p = largest prime <
 *          n.
 * https://oeis.org/A002123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=0, a(2)=0; for n>2, a(n) = -(sum of a(n-p) for all primes
 *            3<=p<=largest_prime_less_than_n) + (n if n is prime else 0)
 * Range:     i8 n <= 18, i16 n <= 39, i32 n <= 82, i64 n <= 167,
 *            i128 n <= 336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

static uint64_t largest_prime_below(uint64_t n) {
    if (n <= 2) return 0;
    uint64_t candidate = (n % 2 == 0) ? n - 1 : n - 2;
    while (candidate >= 2) {
        if (is_prime(candidate)) return candidate;
        candidate -= 2;
    }
    return 2;
}

static struct {
    seq_t *vals;
    unsigned char *computed;
    idx_t len;
} memo = {NULL, NULL, 0};

static seq_t compute_a(idx_t n);

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (memo.vals == NULL) {
        memo.len = (n < 1000) ? 1000 : n + 100;
        memo.vals = (seq_t *)calloc(memo.len, sizeof(seq_t));
        memo.computed = (unsigned char *)calloc(memo.len, sizeof(unsigned char));
        if (memo.vals == NULL || memo.computed == NULL) return 0;
    } else if (n >= memo.len) {
        idx_t new_len = n + 100;
        seq_t *new_vals = (seq_t *)realloc(memo.vals, new_len * sizeof(seq_t));
        unsigned char *new_computed = (unsigned char *)realloc(memo.computed, new_len * sizeof(unsigned char));
        if (new_vals == NULL || new_computed == NULL) return 0;
        
        memset(new_vals + memo.len, 0, (new_len - memo.len) * sizeof(seq_t));
        memset(new_computed + memo.len, 0, (new_len - memo.len) * sizeof(unsigned char));
        
        memo.vals = new_vals;
        memo.computed = new_computed;
        memo.len = new_len;
    }
    
    if (!memo.computed[n]) {
        memo.vals[n] = compute_a(n);
        memo.computed[n] = 1;
    }
    
    return memo.vals[n];
}

static seq_t compute_a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1 || n == 2) return 0;
    
    seq_t result = 0;
    uint64_t largest_p = largest_prime_below((uint64_t)n);
    
    for (uint64_t p = 3; p <= largest_p; p++) {
        if (is_prime(p)) {
            idx_t idx = (idx_t)(n - p);
            if (idx >= 1) {
                result -= a(idx);
            }
        }
    }
    
    if (is_prime((uint64_t)n)) {
        result += (seq_t)n;
    }
    
    return result;
}
