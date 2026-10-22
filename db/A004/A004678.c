/*
 * A004678  Primes written in base 4.
 * https://oeis.org/A004678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007090(A000040(n)) Converts the n-th prime number to
 *            base 4 and interprets the digits as decimal
 * Range:     u8 n <= 15, u16 n <= 172, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool is_prime(uint64_t m) {
    if (m < 2) return false;
    if (m == 2) return true;
    if (m % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) return false;
    }
    return true;
}

static uint64_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    
    uint64_t count = 1;
    uint64_t candidate = 3;
    
    while (true) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate += 2;
    }
}

seq_t a(idx_t n) {
    uint64_t prime = nth_prime(n);
    
    seq_t result = 0;
    seq_t base10_power = 1;
    
    uint64_t temp = prime;
    while (temp > 0) {
        uint64_t digit = temp % 4;
        result = result + ((seq_t)digit) * base10_power;
        base10_power = base10_power * 10;
        temp = temp / 4;
    }
    
    return result;
}
