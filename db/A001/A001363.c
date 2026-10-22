/*
 * A001363  Primes in ternary.
 * https://oeis.org/A001363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = A007089(A000040(n)) - read n-th prime in base 3 as
 *            decimal
 * Range:     u8 n <= 9, u16 n <= 53, u32 n <= 847, u64 n <= 847,
 *            u128 n <= 847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    if (p < 9) return true;
    if (p % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= p; i += 6) {
        if (p % i == 0 || p % (i + 2) == 0) return false;
    }
    return true;
}

static uint64_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    uint64_t count = 1;
    uint64_t candidate = 3;
    while (1) {
        if (is_prime(candidate)) {
            count++;
            if (count == (uint64_t)n) {
                return candidate;
            }
        }
        candidate += 2;
    }
}

seq_t a(idx_t n) {
    uint64_t prime = nth_prime(n);
    
    seq_t result = 0;
    seq_t power = 1;
    
    while (prime > 0) {
        seq_t digit = (seq_t)(prime % 3);
        result = result + digit * power;
        power = power * 10;
        prime = prime / 3;
    }
    
    return result;
}
