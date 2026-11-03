/*
 * A002656  Expansion of (eta(q) * eta(q^7))^3 in powers of q.
 * https://oeis.org/A002656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   Multiplicative function from (eta(q)*eta(q^7))^3 expansion;
 *            a(7^e) = (-7)^e; a(p^e) = 0 if p ≡ 3,5,6 (mod 7) and e odd, else
 *            p^e; for p ≡ 1,2,4 (mod 7): a(p) = 2*(x^2 - 7*y^2) where p = x^2
 *            + 7*y^2, and a(p^e) = a(p)*a(p^(e-1)) - p^2*a(p^(e-2)) for e > 1
 * Range:     i8 n <= 57, i16 n <= 6553, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9715 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

static int find_representation(uint64_t p, uint64_t *x_out, uint64_t *y_out) {
    for (uint64_t y = 0; 7 * y * y <= p; y++) {
        uint64_t diff = p - 7 * y * y;
        uint64_t x = isqrt_u64(diff);
        if (x * x == diff) {
            *x_out = x;
            *y_out = y;
            return 1;
        }
    }
    return 0;
}

static int64_t compute_prime_power_i64(uint64_t p, uint64_t e) {
    if (p == 7) {
        int64_t result = 1;
        int64_t base = -7;
        for (uint64_t i = 0; i < e; i++) {
            result *= base;
        }
        return result;
    }
    
    int mod = (int)(p % 7);
    if (mod == 3 || mod == 5 || mod == 6) {
        if (e % 2 == 1) return 0;
        int64_t result = 1;
        for (uint64_t i = 0; i < e; i++) {
            result *= (int64_t)p;
        }
        return result;
    }
    
    if (p == 2) {
        if (e == 0) return 1;
        if (e == 1) return -3;
        
        int64_t prev2 = 1;
        int64_t prev1 = -3;
        
        for (uint64_t i = 2; i <= e; i++) {
            int64_t curr = prev1 * (-3) - 4 * prev2;
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
    
    uint64_t x, y;
    if (!find_representation(p, &x, &y)) {
        return 0;
    }
    
    int64_t a_p = 2 * ((int64_t)x * (int64_t)x - 7 * (int64_t)y * (int64_t)y);
    
    if (e == 1) return a_p;
    
    int64_t prev2 = 1;
    int64_t prev1 = a_p;
    
    for (uint64_t i = 2; i <= e; i++) {
        int64_t p2 = (int64_t)p * (int64_t)p;
        int64_t curr = prev1 * a_p - p2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    int64_t result = 1;
    uint64_t temp = (uint64_t)n;
    
    for (uint64_t p = 2; p * p <= temp; p++) {
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        if (e > 0) {
            int64_t contrib = compute_prime_power_i64(p, e);
            result *= contrib;
        }
    }
    
    if (temp > 1) {
        int64_t contrib = compute_prime_power_i64(temp, 1);
        result *= contrib;
    }
    
    return (seq_t)result;
}
