/*
 * A004681  Primes written in base 7.
 * https://oeis.org/A004681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007093(prime(n)) - primes written in base 7
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    // Find the n-th prime
    uint64_t count = 0;
    uint64_t prime = 1;
    
    while (count < n) {
        prime++;
        if (is_prime(prime)) {
            count++;
        }
    }
    
    // Convert prime to base 7, reading digits as decimal
    seq_t result = 0;
    seq_t power = 1;
    
    while (prime > 0) {
        seq_t digit = (seq_t)(prime % 7);
        result = result + digit * power;
        power = power * 10;
        prime = prime / 7;
    }
    
    return result;
}
