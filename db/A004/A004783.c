/*
 * A004783  Numbers k such that 3!*(2k-4)!/(k!*(k-1)!) is an integer.
 * https://oeis.org/A004783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that 6*(2k-4)!/(k!*(k-1)!) is an integer, via
 *            prime factorization
 * Range:     u8 n <= 37, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   v_p_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/v_p_factorial.h"

// Check if p is prime via trial division
static bool is_prime_simple(idx_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    for (idx_t d = 3; d * d <= p; d += 2) {
        if (p % d == 0) return false;
    }
    return true;
}

// Check if k satisfies the divisibility condition
static bool is_valid(idx_t k) {
    if (k < 2) return false;
    
    idx_t limit = 2 * k - 4;
    
    // For each prime p, verify v_p(6*(2k-4)!) >= v_p(k!*(k-1)!)
    // where v_p(x) is the highest power of p dividing x
    
    // Prime 2: v_2(6) = 1
    idx_t v_2_num = 1 + v_p_factorial(limit, 2);
    idx_t v_2_den = v_p_factorial(k, 2) + v_p_factorial(k - 1, 2);
    if (v_2_num < v_2_den) return false;
    
    // Prime 3: v_3(6) = 1
    idx_t v_3_num = 1 + v_p_factorial(limit, 3);
    idx_t v_3_den = v_p_factorial(k, 3) + v_p_factorial(k - 1, 3);
    if (v_3_num < v_3_den) return false;
    
    // Other primes: v_p(6) = 0
    for (idx_t p = 5; p <= limit; p += 2) {
        if (!is_prime_simple(p)) continue;
        
        idx_t v_p_num = v_p_factorial(limit, p);
        idx_t v_p_den = v_p_factorial(k, p) + v_p_factorial(k - 1, p);
        if (v_p_num < v_p_den) return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 2;
    
    while (count < n) {
        if (is_valid(k)) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k++;
    }
    
    return 0;
}
