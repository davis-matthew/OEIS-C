/*
 * A004682  Primes written in base 8.
 * https://oeis.org/A004682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007094(prime(n)) where A007094 converts a number to base
 *            8 and interprets digits as base 10
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime_check(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n == 3) return true;
    if (n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

static uint64_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    uint64_t count = 1;
    uint64_t candidate = 3;
    while (true) {
        if (is_prime_check(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate += 2;
    }
}

seq_t a(idx_t n) {
    uint64_t prime = nth_prime(n);
    seq_t result = 0;
    seq_t power = 1;
    
    while (prime > 0) {
        seq_t digit = (seq_t)(prime % 8);
        result = result + digit * power;
        power = power * 10;
        prime = prime / 8;
    }
    
    return result;
}
