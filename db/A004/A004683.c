/*
 * A004683  Primes written in base 9.
 * https://oeis.org/A004683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = interpret_base9_as_decimal(n-th prime)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    // Find the n-th prime
    idx_t count = 0;
    uint64_t p = 2;
    
    while (count < n) {
        if (is_prime(p)) {
            count++;
            if (count == n) break;
        }
        p++;
    }
    
    // Convert prime p to base 9 and interpret as decimal
    seq_t result = 0;
    seq_t power = 1;
    
    while (p > 0) {
        seq_t digit = (seq_t)(p % 9);
        result += digit * power;
        power *= 10;
        p /= 9;
    }
    
    return result;
}
