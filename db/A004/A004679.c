/*
 * A004679  Primes written in base 5.
 * https://oeis.org/A004679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007091(A000040(n)) - the nth prime written in base 5
 * Range:     u8 n <= 21, u16 n <= 445, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    // Find the nth prime
    uint64_t count = 0;
    uint64_t candidate = 1;
    
    while (count < n) {
        candidate++;
        if (is_prime(candidate)) {
            count++;
        }
    }
    
    uint64_t prime = candidate;
    
    // Convert prime to base 5 and interpret as decimal
    seq_t result = 0;
    seq_t power_of_10 = 1;
    
    while (prime > 0) {
        uint64_t digit = prime % 5;
        result += (seq_t)digit * power_of_10;
        power_of_10 *= 10;
        prime /= 5;
    }
    
    return result;
}
