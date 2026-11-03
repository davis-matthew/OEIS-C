/*
 * A004680  Primes written in base 6.
 * https://oeis.org/A004680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A007092(prime(n)) - convert the n-th prime to base 6
 * Range:     u8 n <= 28, u16 n <= 985, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 924 (2026-11-03)
 * Helpers:   is_prime_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/nth_prime.h"

static seq_t to_base6_as_decimal(uint64_t num) {
    seq_t result = 0;
    seq_t place = 1;
    
    while (num > 0) {
        seq_t digit = (seq_t)(num % 6);
        result = result + digit * place;
        place = place * 10;
        num /= 6;
    }
    
    return result;
}

seq_t a(idx_t n) {
    uint64_t prime = nth_prime(n);
    return to_base6_as_decimal(prime);
}
