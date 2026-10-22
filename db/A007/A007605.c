/*
 * A007605  Sum of digits of n-th prime.
 * https://oeis.org/A007605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A000040(n)), sum of digits of n-th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

static uint64_t nth_prime(idx_t n) {
    if (n == 1) return 2;
    
    idx_t count = 1;
    uint64_t candidate = 3;
    
    while (1) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate += 2;
    }
}

static seq_t sum_of_digits(uint64_t num) {
    seq_t sum = 0;
    while (num > 0) {
        sum += (seq_t)(num % 10);
        num /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    uint64_t prime = nth_prime(n);
    return sum_of_digits(prime);
}
